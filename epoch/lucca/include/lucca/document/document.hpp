#ifndef LUCCA_DOCUMENT_HPP
#define LUCCA_DOCUMENT_HPP

#include "lucca/config.hpp"

#include <boost/noncopyable.hpp>

#include <string>

namespace lucca {

class View;

/**
 * @author O Programador
 */
class LUCCA_API Document : boost::noncopyable {
public:
	virtual ~Document() = default;

	virtual void setDocumentName(const std::string& name) = 0;

	virtual std::string getDocumentName() const = 0;
	
	virtual void addView(std::unique_ptr<View> view) = 0;

	virtual View* getView() const = 0;

	virtual void postRedisplay() const = 0;

	virtual void startContinuousPostRedisplay() = 0;

	virtual void stopContinuousPostRedisplay() = 0;
};

}

#endif // LUCCA_DOCUMENT_HPP