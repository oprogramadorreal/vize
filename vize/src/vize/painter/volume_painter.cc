#include <GL/glew.h>

#include "vize/painter/volume_painter.hpp"
#include "vize/model/volume.hpp"
#include "vize/opengl/gl_rgb_cube_renderer.hpp"
#include "vize/opengl/gl_volume_texture.hpp"
#include "vize/opengl/gl_volume_texture_cache.hpp"
#include "vize/opengl/gl_framebuffer_texture_2d.hpp"
#include "vize/opengl/gl_ndc_rectangle.hpp"
#include "vize/opengl/gl_transfer_function_texture.hpp"
#include "vize/shader/mpr_ray_casting_program.hpp"
#include "vize/application/ui_colors.hpp"

#include <lucca/camera.hpp>
#include <lucca/render_target.hpp>

#include <ayla/geometry/axis_aligned_box.hpp>

namespace vize {

struct VolumePainter::GLData final {
	std::unique_ptr<MprRayCastingProgram> shader;
	std::unique_ptr<GLRGBCubeRenderer> rgbCube;
	std::unique_ptr<GLNDCRectangle> ndcRectangle;
	std::unique_ptr<GLFramebufferTexture2D> raysBeginTexture;
	std::unique_ptr<GLFramebufferTexture2D> raysEndTexture;
	std::unique_ptr<GLTransferFunctionTexture> transferFunctionTexture;
	std::shared_ptr<GLVolumeTexture> volumeTexture;
};

VolumePainter::VolumePainter() = default;

VolumePainter::VolumePainter(Volume* volume)
	: _volume(volume)
{
	assert(_volume);
}

VolumePainter::~VolumePainter() = default;

void VolumePainter::setVolumeTextureCache(GLVolumeTextureCache* volumeTextureCache) {
	_volumeTextureCache = volumeTextureCache;
}

void VolumePainter::_initializeImpl(lucca::RenderTarget* target) {
	if (!_volume || !_volumeTextureCache) {
		return; // no volume has been provided
	}

	glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
	glEnable(GL_CULL_FACE);

	_gl = std::make_unique<GLData>();

	_gl->shader = std::make_unique<MprRayCastingProgram>();
	_gl->rgbCube = std::make_unique<GLRGBCubeRenderer>();
	
	// create screen rectangle buffers:
	_gl->ndcRectangle = std::make_unique<GLNDCRectangle>();

	// create transfer function 1D texture:
	_gl->transferFunctionTexture = std::make_unique<GLTransferFunctionTexture>();

	// create 3D texture:
	_gl->volumeTexture = _volumeTextureCache->getTexture(*_volume); // TODO: rebuild this texture if volume change

	// create off-screen rendering targets
	_gl->raysBeginTexture = std::make_unique<GLFramebufferTexture2D>(target->getRenderTargetWidth(), target->getRenderTargetHeight());
	_gl->raysEndTexture = std::make_unique<GLFramebufferTexture2D>(target->getRenderTargetWidth(), target->getRenderTargetHeight());
}

void VolumePainter::_disposeImpl() {
	_gl.reset();
}

void VolumePainter::_paintImpl(const lucca::Camera& camera) {
	if (!_gl) {
		return;
	}

	const auto modelMatrix = getModelMatrix();
	const auto viewMatrix = camera.getViewMatrix();
	const auto projectionMatrix = camera.getProjectionMatrix();

	// resize output textures
	_gl->raysBeginTexture->resizeTexture(camera.getViewportAbsoluteWidth(), camera.getViewportAbsoluteHeight());
	_gl->raysEndTexture->resizeTexture(camera.getViewportAbsoluteWidth(), camera.getViewportAbsoluteHeight());

	// render to the "rays end" texture
	glBindFramebuffer(GL_FRAMEBUFFER, _gl->raysEndTexture->getFramebufferID());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	_gl->rgbCube->draw(viewMatrix, projectionMatrix, modelMatrix);

	// render to the "rays begin" texture
	glBindFramebuffer(GL_FRAMEBUFFER, _gl->raysBeginTexture->getFramebufferID());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
	_gl->rgbCube->draw(viewMatrix, projectionMatrix, modelMatrix);

	// render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, camera.getDefaultFramebufferObject());

	glUseProgram(_gl->shader->getId());

	const auto cameraPositionWorldSpace = camera.getPosition();
	const auto cameraPositionNormalizedModelSpace = _toNormalizedModelSpace(cameraPositionWorldSpace);

	const auto volumeSpacing = _calculateVolumeSpacing();

	glUniform3fv(_gl->shader->getUniform_cameraPosition_normmodelspace(), 1, &cameraPositionNormalizedModelSpace.x);
	glUniform3fv(_gl->shader->getUniform_lightPosition_normmodelspace(), 1, &cameraPositionNormalizedModelSpace.x);
	glUniform1i(_gl->shader->getUniform_lightingEnabled(), int(_lightingEnabled));
	glUniform3fv(_gl->shader->getUniform_volumeSpacing(), 1, &volumeSpacing.x);
	glUniform1f(_gl->shader->getUniform_xPlanePosition(), _xPlanePosition);
	glUniform1f(_gl->shader->getUniform_yPlanePosition(), _yPlanePosition);
	glUniform1f(_gl->shader->getUniform_zPlanePosition(), _zPlanePosition);
	glUniform1i(_gl->shader->getUniform_cuttingPlanes(), int(_cuttingPlanesEnabled));
	glUniform1f(_gl->shader->getUniform_sampleStep(), _samplesStep);
	const auto backgroundColor = UiColors::getRayCastringBackgroundColor() / glm::vec3(255.0f);
	glUniform3fv(_gl->shader->getUniform_backgroundColor(), 1, &backgroundColor.x);

	// bind volume 3D texture in texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, _gl->volumeTexture->getID());
	glUniform1i(_gl->shader->getUniform_volumeSampler(), 0);

	// bind volume 1D texture in texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, _gl->transferFunctionTexture->getID());

	// TODO: optimize this - it is not necessary to send the transfer function at every frame
	const auto sampledTransferFunction = _transferFunction.sampleAlongFunction();
	glTexImage1D(GL_TEXTURE_1D, GLint(0), GL_RGBA8, GLsizei(sampledTransferFunction.size()), GLint(0), GL_RGBA, GL_UNSIGNED_BYTE, (void*)sampledTransferFunction.data());

	glUniform1i(_gl->shader->getUniform_transferFunction(), 1);

	// bind "rays origin" texture in texture unit 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _gl->raysBeginTexture->getTextureID());
	glUniform1i(_gl->shader->getUniform_raysOriginTexture(), 2);

	// bind "rays end" texture in texture unit 3
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _gl->raysEndTexture->getTextureID());
	glUniform1i(_gl->shader->getUniform_raysEndTexture(), 3);

	_gl->ndcRectangle->drawElements(0);
}

void VolumePainter::_resizeImpl(SizeType, SizeType) { }

ayla::AxisAlignedBox VolumePainter::_getAABBImpl() const {
	if (_volume) {
		return _volume->getAABB();
	}

	return ayla::AxisAlignedBox();
}

std::unique_ptr<lucca::Painter> VolumePainter::_cloneImpl() const {
	auto copy = std::make_unique<VolumePainter>();

	copy->_volume = _volume;
	copy->_volumeTextureCache = _volumeTextureCache;
	copy->_transferFunction = _transferFunction;
	copy->_xPlanePosition = _xPlanePosition;
	copy->_yPlanePosition = _yPlanePosition;
	copy->_zPlanePosition = _zPlanePosition;
	copy->_cuttingPlanesEnabled = _cuttingPlanesEnabled;
	copy->_lightingEnabled = _lightingEnabled;
	copy->_samplesStep = _samplesStep;

	return std::move(copy);
}

glm::vec3 VolumePainter::_toNormalizedModelSpace(const glm::vec3& pointWorld) const {
	const auto inverseModelMatrix = glm::inverse(getModelMatrix());
	return glm::vec3(inverseModelMatrix * glm::vec4(pointWorld, 1.0f)) + glm::vec3(0.5f);
}

glm::vec3 VolumePainter::_calculateVolumeSpacing() const {
	if (!_volume) {
		return glm::vec3(0.0f);
	}

	const auto volumeSize = 0.5f * glm::vec3(
		_volume->getWidth(),
		_volume->getHeight(),
		_volume->getDepth()
	);

	auto volumeSpacing = glm::vec3(0.0f);

	for (auto i = SizeType(0); i < SizeType(3); ++i) {
		volumeSpacing[i] = ayla::isZero(volumeSize[i]) ? 0.0f : (1.0f / volumeSize[i]);
	}

	return volumeSpacing;
}

void VolumePainter::setTransferFunction(const TransferFunction& function) {
	_transferFunction = function;
}

TransferFunction VolumePainter::getTransferFunction() const { return _transferFunction; }

void VolumePainter::setXPlanePosition(Float position) { _xPlanePosition = position; }
void VolumePainter::setYPlanePosition(Float position) { _yPlanePosition = position; }
void VolumePainter::setZPlanePosition(Float position) { _zPlanePosition = position; }

Float VolumePainter::getXPlanePosition() const { return _xPlanePosition; }
Float VolumePainter::getYPlanePosition() const { return _yPlanePosition; }
Float VolumePainter::getZPlanePosition() const { return _zPlanePosition; }

void VolumePainter::setCuttingPlanesEnabled(bool enabled) { _cuttingPlanesEnabled = enabled; }
bool VolumePainter::getCuttingPlanesEnabled() const { return _cuttingPlanesEnabled; }

void VolumePainter::setLightingEnabled(bool enabled) { _lightingEnabled = enabled; }
bool VolumePainter::getLightingEnabled() const { return _lightingEnabled; }

void VolumePainter::setSampleStep(Float step) { _samplesStep = step; }
Float VolumePainter::getSampleStep() const { return _samplesStep; }

void VolumePainter::setModelMatrix(const glm::mat4& modelMatrix) {
	if (_volume) {
		_volume->setModelMatrix(modelMatrix);
	}
}
glm::mat4 VolumePainter::getModelMatrix() const {
	return _volume ? _volume->getModelMatrix() : glm::mat4(1.0f);
}

const VolumeHistogram* VolumePainter::getHistogram() const {
	return _volume ? _volume->getHistogram() : nullptr;
}

}