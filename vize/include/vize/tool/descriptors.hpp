#ifndef VIZE_TOOLS_DESCRIPTORS_HPP
#define VIZE_TOOLS_DESCRIPTORS_HPP

#include "vize/config.hpp"

#include <lucca/tool/tool_descriptor.hpp>

namespace lucca {
	class LookBackTool;
	class LookFrontTool;
	class LookLeftTool;
	class LookRightTool;
	class LookTopTool;
	class LookBottomTool;
	class DuplicateSubViewTool;
}

namespace vize {
	class OpenDocumentTool;
	class SaveAsDocumentTool;
	class ShowAboutTool;
	class ImportDicomTool;
	class ImportTiffTool;
	class ImportRawTool;
	class LightSwitchTool;
	class ModelTransformerSwitchTool;
}

namespace descriptor_declaration {

template <>
class ToolDescriptor<lucca::LookBackTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "Back"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/look_back"; }
	virtual std::string getToolTip() const final override { return "Look at the back."; }
};

template <>
class ToolDescriptor<lucca::LookFrontTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "Front"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/look_front"; }
	virtual std::string getToolTip() const final override { return "Look at the front."; }
};

template <>
class ToolDescriptor<lucca::LookLeftTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "Left"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/look_left"; }
	virtual std::string getToolTip() const final override { return "Look at the left."; }
};

template <>
class ToolDescriptor<lucca::LookRightTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "Right"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/look_right"; }
	virtual std::string getToolTip() const final override { return "Look at the right."; }
};

template <>
class ToolDescriptor<lucca::LookTopTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "Top"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/look_top"; }
	virtual std::string getToolTip() const final override { return "Look at the top."; }
};

template <>
class ToolDescriptor<lucca::LookBottomTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "Bottom"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/look_bottom"; }
	virtual std::string getToolTip() const final override { return "Look at the bottom."; }
};

template <>
class ToolDescriptor<lucca::DuplicateSubViewTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "Duplicate View"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/plus"; }
	virtual std::string getToolTip() const final override { return "Duplicate this view."; }
};

template <>
class ToolDescriptor<vize::OpenDocumentTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "Open"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/open"; }
	virtual std::string getToolTip() const final override { return "Open a Vize document."; }
};

template <>
class ToolDescriptor<vize::SaveAsDocumentTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "Save As..."; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/save_as"; }
	virtual std::string getToolTip() const final override { return "Save Vize document."; }
};

template <>
class ToolDescriptor<vize::ShowAboutTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "About"; }
	virtual std::string getIcon() const final override { return ":resources/vize"; }
	virtual std::string getToolTip() const final override { return "About Vize."; }
};

template <>
class ToolDescriptor<vize::ImportDicomTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "DICOM"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/import_dicom"; }
	virtual std::string getToolTip() const final override { return "Import DICOM volume."; }
};

template <>
class ToolDescriptor<vize::ImportTiffTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "TIFF"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/import_tiff"; }
	virtual std::string getToolTip() const final override { return "Import TIFF volume."; }
};

template <>
class ToolDescriptor<vize::ImportRawTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "RAW"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/import_raw"; }
	virtual std::string getToolTip() const final override { return "Import RAW volume."; }
};

template <>
class ToolDescriptor<vize::LightSwitchTool> final : public lucca::ToolDescriptor {
private:
	virtual std::string getName() const final override { return "Light Switch"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/light"; }
	virtual std::string getToolTip() const final override { return "Enable/disable lighting."; }
};

template <>
class ToolDescriptor<vize::ModelTransformerSwitchTool> final : public lucca::ToolDescriptor{
private:
	virtual std::string getName() const final override { return "Model Transformer Switch"; }
	virtual std::string getIcon() const final override { return ":resources/icons/16/transform"; }
	virtual std::string getToolTip() const final override { return "Show/hide model transformer."; }
};

}

#endif // VIZE_TOOLS_DESCRIPTORS_HPP