#ifndef VIZE_IMPORT_VOLUME_LAUNCHER_HPP
#define VIZE_IMPORT_VOLUME_LAUNCHER_HPP

#include "vize/config.hpp"

#include "vize/application/application_launcher.hpp"
#include "vize/factory/async_volume_document_factory.hpp"

namespace vize {

class VolumePainterFactory;
class SubViewFactory;

/**
 * @author O Programador
 */
class ImportVolumeLauncher : public ApplicationLauncher {
public:
	ImportVolumeLauncher(
		VolumeDocument& document,
		std::unique_ptr<VolumeFactory> volumeFactory,
		std::unique_ptr<VolumePainterFactory> volumePainterFactory,
		std::unique_ptr<SubViewFactory> subViewFactory
	);

	virtual ~ImportVolumeLauncher() = 0;

private:
	virtual void run() final override;
	virtual bool isRunning() const final override;
	virtual void showProgressDialog(const std::string& progressDialogTitle) final override;

private:
	void _documentBuilt(VolumeDocument& document, Volume& volume);

private:
	virtual std::string _getVolumeDirectory() const = 0;
	virtual void _documentBuiltImpl(const VolumeDocument& document, Volume& volume) = 0;

private:
	AsyncVolumeDocumentFactory _documentFactory;
	boost::signals2::scoped_connection _documentFactoryConnection;
	std::unique_ptr<VolumePainterFactory> _volumePainterFactory;
	std::unique_ptr<SubViewFactory> _subViewFactory;
};

/**
  * @author O Programador
  */
class ImportCTHEAD final : public ImportVolumeLauncher {
public:
	ImportCTHEAD(VolumeDocument& document, std::unique_ptr<VolumePainterFactory> volumePainterFactory, std::unique_ptr<SubViewFactory> subViewFactory);

private:
	virtual std::string _getVolumeDirectory() const final override;
	virtual void _documentBuiltImpl(const VolumeDocument& document, Volume& volume) final override;
};

/**
 * @author O Programador
 */
class ImportMANIX final : public ImportVolumeLauncher {
public:
	ImportMANIX(VolumeDocument& document, std::unique_ptr<VolumePainterFactory> volumePainterFactory, std::unique_ptr<SubViewFactory> subViewFactory);

private:
	virtual std::string _getVolumeDirectory() const final override;
	virtual void _documentBuiltImpl(const VolumeDocument& document, Volume& volume) final override;
};

}

#endif // VIZE_IMPORT_VOLUME_LAUNCHER_HPP