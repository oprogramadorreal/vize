#ifndef VIZE_IMPORT_VOLUME_TOOL_HPP
#define VIZE_IMPORT_VOLUME_TOOL_HPP

#include "vize/config.hpp"

#include "vize/factory/async_volume_document_factory.hpp"

#include <lucca/tool/tool_impl.hpp>

class QStringList;

namespace vize {

/**
 * @see vize::ImportDicomTool
 * @see vize::ImportTiffTool
 * @see vize::ImportRawTool
 *
 * @author O Programador
 */
class ImportVolumeTool : public lucca::ToolImpl {
public:
	ImportVolumeTool(VolumeDocument& document, std::unique_ptr<VolumeFactory> volumeFactory);
	virtual ~ImportVolumeTool() = 0;

protected:
	void _buildDocument(const QStringList& volumeFiles, const std::string& loadingDialogTitle);

private:
	void _documentBuilt(VolumeDocument& document, Volume& volume);

private:
	VolumeDocument& _document;
	AsyncVolumeDocumentFactory _documentFactory;
	boost::signals2::scoped_connection _documentFactoryConnection;
};

}

#endif // VIZE_IMPORT_VOLUME_TOOL_HPP