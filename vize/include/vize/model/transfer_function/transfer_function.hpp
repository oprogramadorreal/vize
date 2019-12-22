#ifndef VIZE_TRANSFER_FUNCTION_HPP
#define VIZE_TRANSFER_FUNCTION_HPP

#include "vize/config.hpp"

#include "vize/model/transfer_function/control_point.hpp"
#include "vize/serialization/transfer_function_serializer.hpp"

#include <vector>

namespace vize {

/**
 * Transfer function used in ray casting algorithm.
 *
 * @author O Programador
 */
class TransferFunction final {
public:
	TransferFunction();

	/**
	 * @param position Must be a value betwwen 0 and 1.
	 *
	 * @return Interpolated color at @param position.
	 */
	Color sample(Float position) const;

	/**
	 * Collect many samples along this function.
	 *
	 * @return A vector containing each sampled color.
	 */
	std::vector<Color> sampleAlongFunction(SizeType numberOfSteps = SizeType(1025)) const;

	/**
	 * @return The number of control points in this function.
	 *
	 * @remarks It always has at least two control points at positions 0 and 1.
	 */
	SizeType getNumberOfPoints() const;

	/**
	 * @param pointIndex An index of one control point in this function.
	 * @param newPosition The new position of this point.
	 */
	void setPointPosition(SizeType pointIndex, Float newPosition);

	/**
	 * @param pointIndex An index of one control point in this function.
	 * @param newColor The new color of this point.
	 */
	void setPointColor(SizeType pointIndex, const Color& newColor);

	/**
	 * @return The position of the specified point in this function.
	 */
	Float getPointPosition(SizeType pointIndex) const;

	/**
	 * @return The color of the specified point of this function.
	 */
	Color getPointColor(SizeType pointIndex) const;

	/**
	 * @return A bidimensional representation for the position of a point in this function.
	 * Both x and y coordinates will be between 0 and 1.
	 */
	glm::vec2 getPointNormalized2DPosition(SizeType pointIndex) const;

	/**
	 * //
	 */
	void setPointNormalized2DPosition(SizeType pointIndex, const glm::vec2& normalized2dPosition);

	/**
	 * //
	 *
	 * @return New point index.
	 */
	SizeType addPoint(Float position, const Color& color);

	/**
	 * //
	 */
	bool removePoint(SizeType pointIndex);

private:
	using PointsContainer = std::vector<ControlPoint>;
	using PositionInterval = std::pair<PointsContainer::const_iterator, PointsContainer::const_iterator>;

	PositionInterval _getInterval(Float position) const;

private:
	PointsContainer _controlPoints;

private:
	template<class Archive> friend void boost::serialization::serialize(Archive&, vize::TransferFunction&, const unsigned int);
	template<class Archive> friend void boost::serialization::load_construct_data(Archive&, vize::TransferFunction*, const unsigned int);
};

}

#endif // VIZE_TRANSFER_FUNCTION_HPP