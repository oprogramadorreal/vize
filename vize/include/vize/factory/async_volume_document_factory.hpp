#ifndef VIZE_ASYNC_VOLUME_DOCUMENT_FACTORY_HPP
#define VIZE_ASYNC_VOLUME_DOCUMENT_FACTORY_HPP

#include "vize/config.hpp"

#include "vize/factory/async_volume_factory.hpp"

namespace vize {

class VolumeDocument;

/**
 * @author O Programador
 */
class AsyncVolumeDocumentFactory final {
public:
	AsyncVolumeDocumentFactory(VolumeDocument& document, std::unique_ptr<VolumeFactory> factory);
	virtual ~AsyncVolumeDocumentFactory();

public:
	void run();

	bool isRunning() const;

	void showProgressDialog(const std::string& progressDialogTitle);

	void addVolumeImagesDirectory(const std::string& imagesDirectory);
	void addVolumeImage(const std::string& imageFile);
	void clearVolumeImages();

public:
	using DocumentBuiltSignal = boost::signals2::signal<void(VolumeDocument&, Volume&)>;
	using DocumentBuiltSignalListener = DocumentBuiltSignal::slot_function_type;

	/**
	* Register a listener to be notified when a volume is built.
	*/
	boost::signals2::connection connectToDocumentBuiltSignal(DocumentBuiltSignalListener listener);

private:
	void _volumeBuilt(const std::shared_ptr<Volume>& volume, const std::string& volumeName);

private:
	VolumeDocument& _document;
	AsyncVolumeFactory _volumeFactory;
	DocumentBuiltSignal _documentBuilt;
};

}

#endif // VIZE_ASYNC_VOLUME_DOCUMENT_FACTORY_HPP