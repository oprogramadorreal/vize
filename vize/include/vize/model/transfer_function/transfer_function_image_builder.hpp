#ifndef VIZE_TRANSFER_FUNCTION_IMAGE_BUILDER_HPP
#define VIZE_TRANSFER_FUNCTION_IMAGE_BUILDER_HPP

#include "vize/config.hpp"

#include "vize/model/transfer_function/transfer_function.hpp"
#include "vize/model/volume_histogram.hpp"

#include <QPixmap>
#include <QPainter>

#include <set>

namespace vize {

/**
 * Knows how to create an image representation for a transfer function.
 *
 * @see vize::TransferFunction
 *
 * @author O Programador
 */
class TransferFunctionImageBuilder final {
public:
	TransferFunctionImageBuilder();

	QPixmap build(const TransferFunction& tFunction, const QSize& imageSize, Float pointRadius) const;

	/**
	 * Converts a control point position into image position coordinates.
	 *
	 * @return The position on the image rectangle where this builder will paint
	 * a control point @param pointIndex of a transfer function @param tFunction.
	 */
	glm::vec2 functionToImage(SizeType pointIndex, const TransferFunction& tFunction, const QSize& imageSize, Float pointRadius) const;

	/**
	 * //
	 */
	glm::vec2 imageToFunction(const glm::vec2& pointOnImage, const QSize& imageSize, Float pointRadius) const;

	void addHighlightedPoints(SizeType pointIndex);
	void clearHighlightedPoints();

	void showHistogram(const VolumeHistogram* histogram);
	void hideHistogram();

private:
	glm::vec2 _toRectCoordinates(const QRect& rect, const glm::vec2& normalized2DPointPosition) const;
	QRect _getDrawingRect(const QSize& imageSize, Float pointRadius) const;

private:
	void _drawBackground(QPainter& painter, const QRect& drawingRect, const QSize& imageSize) const;
	void _drawColorGradient(QPainter& painter, const QRect& drawingRect, const TransferFunction& tFunction) const;
	void _drawLineConnectingPoints(QPainter& painter, const QRect& drawingRect, const TransferFunction& tFunction) const;
	void _drawControlPoints(QPainter& painter, const QRect& drawingRect, const TransferFunction& tFunction, Float pointRadius) const;
	void _drawHistogram(QPainter& painter, const QRect& drawingRect, const VolumeHistogram& histogram) const;

private:
	QPixmap _transparentImage;
	std::set<SizeType> _highlightedPoints;
	const VolumeHistogram* _histogram = nullptr;
};

}

#endif // VIZE_TRANSFER_FUNCTION_IMAGE_BUILDER_HPP