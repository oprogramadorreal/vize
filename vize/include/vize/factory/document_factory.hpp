#ifndef VIZE_DOCUMENT_FACTORY_HPP
#define VIZE_DOCUMENT_FACTORY_HPP

#include "vize/config.hpp"

#include <string>

namespace lucca {
	class MultiViewDocument;
}

namespace vize {

/**
 * @author O Programador
 */
class DocumentFactory final {
public:
	std::unique_ptr<lucca::MultiViewDocument> create(const std::string& documentName) const;
};

}

#endif // VIZE_DOCUMENT_FACTORY_HPP