#ifndef VIZE_TRANSFER_FUNCTION_EDITOR_HPP
#define VIZE_TRANSFER_FUNCTION_EDITOR_HPP

#include "vize/config.hpp"

namespace vize {

class TransferFunction;

/**
 * @author O Programador
 */
class TransferFunctionEditor {
public:
	virtual ~TransferFunctionEditor();

public:
	virtual TransferFunction getTransferFunction() const = 0;
	virtual void setTransferFunction(const TransferFunction& function) = 0;
};

}

#endif // VIZE_TRANSFER_FUNCTION_EDITOR_HPP