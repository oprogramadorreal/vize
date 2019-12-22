#ifndef VIZE_VOLUME_DOCUMENT_HPP
#define VIZE_VOLUME_DOCUMENT_HPP

#include "vize/config.hpp"

#include "vize/serialization/volume_document_serializer.hpp"

#include <lucca_qt/simple_multi_view_document.hpp>

namespace lucca {
	class ViewComposite;
}

namespace vize {

class Volume;
class GLVolumeTextureCache;

/**
 * @author O Programador
 */
class VolumeDocument final : public lucca_qt::SimpleMultiViewDocument {
public:
	VolumeDocument(const std::string& documentName, std::unique_ptr<lucca::ViewComposite> viewComposite);
	virtual ~VolumeDocument();

	void setVolume(const std::shared_ptr<Volume>& volume);
	void setVolumeTextureCache(const std::shared_ptr<GLVolumeTextureCache>& volumeTextureCache);

	glm::mat4 getModelMatrix() const;
	void setModelMatrix(const glm::mat4& modelMatrix);

	SizeType getVolumeWidth() const;
	SizeType getVolumeHeight() const;
	SizeType getVolumeDepth() const;

public: // lucca::Document interface
	virtual void setDocumentName(const std::string& name) final override;
	virtual std::string getDocumentName() const final override;
	virtual void addView(std::unique_ptr<lucca::View> view) final override;

private:
	std::string _documentName;
	std::shared_ptr<Volume> _volume;
	std::shared_ptr<GLVolumeTextureCache> _volumeTextureCache;

	template<class Archive> friend void boost::serialization::serialize(Archive&, vize::VolumeDocument&, const unsigned int);
};

}

#endif // VIZE_VOLUME_DOCUMENT_HPP