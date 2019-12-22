#include "vize/serialization/vize_serializer.hpp"

#include "vize/document/volume_document.hpp"

VizeSerializer::VizeSerializer()
	: _serializer([](){ return std::string("383juj30g8485jgjmsn374"); })
{ }

VizeSerializer::~VizeSerializer() = default;

VizeSerializer::Status VizeSerializer::getStatus() const {
	return _serializer.getStatus();
}

bool VizeSerializer::save(const vize::VolumeDocument& document, const std::string& outputFileName) {
	return _serializer.saveObject(document, outputFileName);
}

bool VizeSerializer::load(vize::VolumeDocument& document, const std::string& inputFileName) {
	return _serializer.loadObject(document, inputFileName);
}