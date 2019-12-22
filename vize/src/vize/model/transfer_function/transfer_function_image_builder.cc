#include "vize/model/transfer_function/transfer_function_image_builder.hpp"

#include "vize/application/ui_colors.hpp"

namespace vize {

TransferFunctionImageBuilder::TransferFunctionImageBuilder() {
	_transparentImage = QPixmap(10, 10);

	QPainter p(&_transparentImage);
	p.fillRect(0, 0, 5, 5, Qt::lightGray);
	p.fillRect(5, 5, 5, 5, Qt::lightGray);
	p.fillRect(0, 5, 5, 5, Qt::darkGray);
	p.fillRect(5, 0, 5, 5, Qt::darkGray);
	p.end();
}

glm::vec2 TransferFunctionImageBuilder::_toRectCoordinates(const QRect& rect, const glm::vec2& normalized2DPointPosition) const {
	return glm::vec2(rect.x(), rect.y()) + glm::vec2(normalized2DPointPosition.x * rect.width(), (1.0f - normalized2DPointPosition.y) * rect.height());
}

void TransferFunctionImageBuilder::_drawBackground(QPainter& painter, const QRect& drawingRect, const QSize& imageSize) const {
	const auto backgroundColor = UiColors::getTransferFunctionBarColor();
	painter.fillRect(QRect(QPoint(0, 0), imageSize), QColor(backgroundColor.x, backgroundColor.y, backgroundColor.z));
	painter.drawTiledPixmap(drawingRect, _transparentImage); // draw checkers pattern
}

void TransferFunctionImageBuilder::_drawLineConnectingPoints(QPainter& painter, const QRect& drawingRect, const TransferFunction& tFunction) const {
	const auto penBackup = painter.pen();

	auto pen = penBackup;
	//pen.setWidth(pen.width() * 2);
	pen.setColor(QColor(0, 0, 0));
	painter.setPen(pen);

	for (auto i = SizeType(0); i < tFunction.getNumberOfPoints() - 1; ++i) {
		const auto pointCenter = _toRectCoordinates(drawingRect, tFunction.getPointNormalized2DPosition(i));
		const auto nextPointCenter = _toRectCoordinates(drawingRect, tFunction.getPointNormalized2DPosition(i + 1));

		painter.drawLine(QPoint(pointCenter.x, pointCenter.y), QPoint(nextPointCenter.x, nextPointCenter.y));
	}

	painter.setPen(penBackup);
}

void TransferFunctionImageBuilder::_drawColorGradient(QPainter& painter, const QRect& drawingRect, const TransferFunction& tFunction) const {
	//QLinearGradient colorGrad(0, 0, drawingRect.width(), 0);
	//QGradientStops stops;

	//for (auto i = SizeType(0); i < tFunction.getNumberOfPoints(); ++i) {
	//	const auto pointColor = tFunction.getPointColor(i);
	//	const auto qColor = QColor(pointColor.getR(), pointColor.getG(), pointColor.getB(), pointColor.getA());

	//	stops.push_back(
	//		qMakePair(qreal(tFunction.getPointPosition(i)), qColor)
	//	);
	//}

	//colorGrad.setStops(stops);
	//painter.fillRect(drawingRect, colorGrad);

	const auto imageData = tFunction.sampleAlongFunction(256);
	QImage colorGrad(int(imageData.size()), 1, QImage::Format_RGBA8888);

	for (auto i = SizeType(0); i < SizeType(imageData.size()); ++i) {
		colorGrad.setPixel(i, 0, qRgba(imageData[i].getR(), imageData[i].getG(), imageData[i].getB(), imageData[i].getA()));
	}

	painter.drawImage(drawingRect, colorGrad);
}

void TransferFunctionImageBuilder::_drawControlPoints(QPainter& painter, const QRect& drawingRect, const TransferFunction& tFunction, Float pointRadius) const {
	for (auto i = SizeType(0); i < tFunction.getNumberOfPoints(); ++i) {
		const auto color = tFunction.getPointColor(i);
		const auto qColor = QColor(color.getR(), color.getG(), color.getB(), 255);

		if (_highlightedPoints.find(i) != _highlightedPoints.end()) {
			auto currentPen = painter.pen();
			currentPen.setWidth(currentPen.width() * 2);
			currentPen.setColor(QColor(0, 0, 0));
			painter.setPen(currentPen);
		} else {
			painter.setPen(QColor(0, 0, 0));
		}

		painter.setBrush(qColor);
		const auto pointCenter = _toRectCoordinates(drawingRect, tFunction.getPointNormalized2DPosition(i));

		painter.drawEllipse(QPointF(pointCenter.x, pointCenter.y), qreal(pointRadius), qreal(pointRadius));
	}
}

void TransferFunctionImageBuilder::_drawHistogram(QPainter& painter, const QRect& drawingRect, const VolumeHistogram& histogram) const {
	const auto penBackup = painter.pen();

	auto pen = penBackup;
	//pen.setWidth(pen.width() * 2);
	pen.setColor(QColor(255, 255, 0));
	painter.setPen(pen);

	const auto histogramSize = SizeType(histogram.getMaxVoxelValue() - 1);

	for (auto i = SizeType(0); i < histogramSize; ++i) {
		const auto point = _toRectCoordinates(drawingRect, glm::vec2(i / Float(histogramSize), histogram.getNormalizedValueFrequency(i)));
		const auto nextPoint = _toRectCoordinates(drawingRect, glm::vec2((i + 1) / Float(histogramSize), histogram.getNormalizedValueFrequency(i + 1)));

		painter.drawLine(QPoint(point.x, point.y), QPoint(nextPoint.x, nextPoint.y));
	}

	painter.setPen(penBackup);
}

QRect TransferFunctionImageBuilder::_getDrawingRect(const QSize& imageSize, Float pointRadius) const {
	return QRect(
		int(pointRadius),
		int(pointRadius),
		imageSize.width() - 2 * int(pointRadius),
		imageSize.height() - 2 * int(pointRadius)
	);
}

QPixmap TransferFunctionImageBuilder::build(const TransferFunction& tFunction, const QSize& imageSize, Float pointRadius) const {
	assert(tFunction.getNumberOfPoints() > SizeType(1));

	QPixmap transferFunctionImage(imageSize);
	QPainter painter(&transferFunctionImage);

	const auto drawingRect = _getDrawingRect(imageSize, pointRadius);
	
	_drawBackground(painter, drawingRect, imageSize);
	_drawColorGradient(painter, drawingRect, tFunction);

	if (_histogram) {
		_drawHistogram(painter, drawingRect, *_histogram);
	}

	_drawLineConnectingPoints(painter, drawingRect, tFunction);
	_drawControlPoints(painter, drawingRect, tFunction, pointRadius);

	return transferFunctionImage;
}

glm::vec2 TransferFunctionImageBuilder::functionToImage(SizeType pointIndex, const TransferFunction& tFunction, const QSize& imageSize, Float pointRadius) const {
	const auto drawingRect = _getDrawingRect(imageSize, pointRadius);
	const auto pointNorm = tFunction.getPointNormalized2DPosition(pointIndex);

	return glm::vec2(
		drawingRect.x() + drawingRect.width() * pointNorm.x,
		drawingRect.y() + drawingRect.height() * (1.0f - pointNorm.y)
	);
}

glm::vec2 TransferFunctionImageBuilder::imageToFunction(const glm::vec2& pointOnImage, const QSize& imageSize, Float pointRadius) const {
	const auto drawingRect = _getDrawingRect(imageSize, pointRadius);

	const auto pointOnDrawingRect = glm::vec2(
		(pointOnImage.x - drawingRect.x()) / Float(drawingRect.width()),
		(pointOnImage.y - drawingRect.y()) / Float(drawingRect.height())
	);

	return glm::vec2(pointOnDrawingRect.x, Float(1.0f - pointOnDrawingRect.y));
}

void TransferFunctionImageBuilder::addHighlightedPoints(SizeType pointIndex) {
	_highlightedPoints.insert(pointIndex);
}

void TransferFunctionImageBuilder::clearHighlightedPoints() {
	_highlightedPoints.clear();
}

void TransferFunctionImageBuilder::showHistogram(const VolumeHistogram* histogram) {
	_histogram = histogram;
}

void TransferFunctionImageBuilder::hideHistogram() {
	_histogram = nullptr;
}

}