#include "lucca_qt/image/create_image.hpp"

#include <ayla/image.hpp>

#include <QGLWidget>

namespace lucca_qt {

std::unique_ptr<ayla::Image> createImage(const std::string& imageFile) {
	QImage qtImage;

	if (!qtImage.load(QString::fromStdString(imageFile))) {
		return nullptr;
	}

	const auto qtImageGL = QGLWidget::convertToGLFormat(qtImage);
	const auto qtImageGLRawData = qtImageGL.constBits();
	std::vector<ayla::Image::Byte> rawData(qtImageGL.byteCount());
	std::copy(qtImageGLRawData, qtImageGLRawData + qtImageGL.byteCount(), rawData.data());

	return std::make_unique<ayla::Image>(
		SizeType(qtImageGL.width()), SizeType(qtImageGL.height()), std::move(rawData)
	);
}

}