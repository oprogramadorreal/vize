#include "vize/ui/transfer_function_editor_widget.hpp"
#include "ui_transfer_function_editor_widget.h"

#include "vize/painter/volume_raycaster.hpp"

#include <QMouseEvent>
#include <QColorDialog>

#include <glm/gtx/norm.hpp>

namespace vize {

TransferFunctionEditorWidget::TransferFunctionEditorWidget(VolumeRaycaster& raycaster, QWidget* parent)
	: QWidget(parent), _raycaster(raycaster), ui(std::make_unique<Ui::TransferFunctionEditorWidget>())
{
	ui->setupUi(this);

	setFocusPolicy(Qt::ClickFocus);
}

TransferFunctionEditorWidget::~TransferFunctionEditorWidget() { }

void TransferFunctionEditorWidget::setTransferFunction(const TransferFunction& function) {
	_currentFunction = function;
	update();
}

TransferFunction TransferFunctionEditorWidget::getTransferFunction() const {
	return _currentFunction;
}

void TransferFunctionEditorWidget::_transferFunctionChanged() {
	_raycaster.setTransferFunction(getTransferFunction());
	update();
}

SizeType TransferFunctionEditorWidget::_pickPoint(const TransferFunction& tFunction, const glm::vec2& positionOnWidget) const {
	const auto pointRadius2 = _pointRadius * _pointRadius;

	for (auto i = SizeType(0); i < tFunction.getNumberOfPoints(); ++i) {
		const auto point = _imageBuilder.functionToImage(i, tFunction, size(), _pointRadius * Float(1.35f));

		if (glm::distance2(point, positionOnWidget) <= pointRadius2) {
			return i;
		}
	}

	return std::numeric_limits<SizeType>::max();
}

void TransferFunctionEditorWidget::mousePressEvent(QMouseEvent* e) {
	assert(e);

	const auto eventPosition = glm::vec2(e->x(), e->y());
	_selectedPointIndex = _pickPoint(_currentFunction, eventPosition);

	if (e->button() == Qt::LeftButton) {
		if (_selectedPointIndex >= _currentFunction.getNumberOfPoints()) { // no point selected... create a new one.
			const auto newPointOnFunction = _imageBuilder.imageToFunction(eventPosition, size(), _pointRadius);
			auto newPointColor = _currentFunction.sample(newPointOnFunction.x);

			newPointColor = Color(
				newPointColor.getR(),
				newPointColor.getG(),
				newPointColor.getB(),
				Color::ChannelType(Float(std::numeric_limits<Color::ChannelType>::max()) * newPointOnFunction.y)
			);

			_selectedPointIndex = _currentFunction.addPoint(
				newPointOnFunction.x, newPointColor
			);

			_transferFunctionChanged();
		}
	} else if (e->button() == Qt::RightButton) {
		if (_currentFunction.removePoint(_selectedPointIndex)) {
			_selectedPointIndex = std::numeric_limits<decltype(_selectedPointIndex)>::max();
			_transferFunctionChanged();
		}
	}
}

void TransferFunctionEditorWidget::mouseReleaseEvent(QMouseEvent* e) {
	_selectedPointIndex = std::numeric_limits<decltype(_selectedPointIndex)>::max();
	update();
}

void TransferFunctionEditorWidget::mouseMoveEvent(QMouseEvent* e) {
	if (e->buttons() & Qt::LeftButton) {
		if (_selectedPointIndex < _currentFunction.getNumberOfPoints()) {
			const auto eventPosition = glm::vec2(e->x(), e->y());
			
			_currentFunction.setPointNormalized2DPosition(
				_selectedPointIndex,
				_imageBuilder.imageToFunction(eventPosition, size(), _pointRadius)
			);

			_transferFunctionChanged();
		}
	}
}

void TransferFunctionEditorWidget::mouseDoubleClickEvent(QMouseEvent* e) {
	if (e->button() == Qt::LeftButton) {
		const auto eventPosition = glm::vec2(e->x(), e->y());
		_selectedPointIndex = _pickPoint(_currentFunction, eventPosition);

		if (_selectedPointIndex < _currentFunction.getNumberOfPoints()) {
			const auto qColor = QColorDialog::getColor(Qt::white, this);

			if (qColor.isValid()) {
				const auto newColor = Color(
					Color::ChannelType(qColor.red()),
					Color::ChannelType(qColor.green()),
					Color::ChannelType(qColor.blue()),
					_currentFunction.getPointColor(_selectedPointIndex).getA()
				);

				_currentFunction.setPointColor(_selectedPointIndex, newColor);
				_transferFunctionChanged();
			}
		}
	}
}

void TransferFunctionEditorWidget::_drawOnBackground(const QPixmap& image) {
	QPainter painter(this);
	painter.drawPixmap(rect(), image);
}

void TransferFunctionEditorWidget::paintEvent(QPaintEvent *event) {
	_imageBuilder.clearHighlightedPoints();
	_imageBuilder.addHighlightedPoints(_selectedPointIndex);

	if (const auto histogram = _raycaster.getHistogram()) {
		_imageBuilder.showHistogram(histogram);
	} else {
		_imageBuilder.hideHistogram();
	}

	_drawOnBackground(
		_imageBuilder.build(_currentFunction, size(), Float(_pointRadius))
	);
}

}