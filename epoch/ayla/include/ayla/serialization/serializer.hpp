#ifndef AYLA_SERIALIZER_HPP
#define AYLA_SERIALIZER_HPP

#include "ayla/config.hpp"

#include <fstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

namespace ayla {

/**
 * @todo Provide options to serialize in other file formats (e.g., XML and text).
 *
 * @see ayla::TriangleMeshSerializer
 * @see ayla::TriangleMeshDistanceFieldSerializer
 * 
 * @author O Programador
 */
template <class SeriazableType>
class Serializer {
public:
	using FileHeaderFunction = std::function<std::string()>;

	Serializer(FileHeaderFunction headerFunction)
		: _headerFunction(headerFunction)
	{ }

	enum class Status {
		Success = 0,
		InvalidFileHeader,
		CannotOpenInputFile,
		CannotOpenOutputFile
	};

	/**
	 * Save @param object into the file specified by @param outputFileName.
	 */
	bool saveObject(const SeriazableType& object, const std::string& outputFileName) {
		std::ofstream stream(outputFileName, std::ios::binary);

		if (!stream.is_open()) {
			_status = Status::CannotOpenOutputFile;
		} else {
			boost::archive::binary_oarchive ar(stream);

			std::string header = _headerFunction();
			ar & boost::serialization::make_nvp("header", header);
			ar & boost::serialization::make_nvp("object", object);
		}

		return _status == Status::Success;
	}

	/**
	 * Load the object saved in the file specified by @param inputFileName into @param object.
	 */
	bool loadObject(SeriazableType& object, const std::string& inputFileName) {
		std::ifstream stream(inputFileName, std::ios::binary);

		if (!stream.is_open()) {
			_status = Status::CannotOpenInputFile;
		} else {
			boost::archive::binary_iarchive ar(stream);

			std::string header;
			ar & boost::serialization::make_nvp("header", header);

			if (header.compare(_headerFunction()) != 0) {
				_status = Status::InvalidFileHeader;
			} else {
				ar & boost::serialization::make_nvp("object", object);
			}
		}

		return _status == Status::Success;
	}

	/**
	 * @return The status of the last operation performed by the serializer.
	 */
	Status getStatus() const {
		return _status;
	}

private:
	FileHeaderFunction _headerFunction;
	Status _status = Status::Success;
};

}

#endif // AYLA_SERIALIZER_HPP