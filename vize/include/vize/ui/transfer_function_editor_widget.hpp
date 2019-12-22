#ifndef VIZE_TRANSFER_FUNCTION_EDITOR_WIDGET_HPP
#define VIZE_TRANSFER_FUNCTION_EDITOR_WIDGET_HPP

#include "vize/config.hpp"

#include "vize/model/transfer_function/transfer_function_editor.hpp"
#include "vize/model/transfer_function/transfer_function_image_builder.hpp"

#include <QWidget>

namespace Ui {
	class TransferFunctionEditorWidget;
}

namespace vize {

class VolumeRaycaster;

/**
 * @author O Programador
 */
class TransferFunctionEditorWidget final : public QWidget, public TransferFunctionEditor {
public:
	explicit TransferFunctionEditorWidget(VolumeRaycaster& raycaster, QWidget* parent);
	virtual ~TransferFunctionEditorWidget();

public:
	virtual TransferFunction getTransferFunction() const override;
	virtual void setTransferFunction(const TransferFunction& function) override;

private:
	virtual void paintEvent(QPaintEvent *event) override;

private:
	virtual void mousePressEvent(QMouseEvent* e) override;
	virtual void mouseReleaseEvent(QMouseEvent* e) override;
	virtual void mouseMoveEvent(QMouseEvent* e) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* e) override;

private:
	void _transferFunctionChanged();
	
private:
	SizeType _pickPoint(const TransferFunction& tFunction, const glm::vec2& positionOnWidget) const;

	void _drawOnBackground(const QPixmap& image);

private:
	VolumeRaycaster& _raycaster;
	TransferFunction _currentFunction;
	SizeType _selectedPointIndex = std::numeric_limits<SizeType>::max();
	SizeType _pointRadius = SizeType(5);
	TransferFunctionImageBuilder _imageBuilder;
	std::unique_ptr<Ui::TransferFunctionEditorWidget> ui;
};

}

#endif // VIZE_TRANSFER_FUNCTION_EDITOR_WIDGET_HPP