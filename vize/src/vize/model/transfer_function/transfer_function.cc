#include "vize/model/transfer_function/transfer_function.hpp"

#include <ayla/interpolation.hpp>

namespace vize {

TransferFunction::TransferFunction() {
	const auto black = Color::black();
	const auto colorAt0 = Color(black.getR(), black.getG(), black.getB(), Color::ChannelType(0));
	const auto colorAt1 = Color::white();

	_controlPoints.push_back(ControlPoint(0.0f, colorAt0));
	_controlPoints.push_back(ControlPoint(1.0f, colorAt1));

	assert(getNumberOfPoints() == SizeType(2));
}

TransferFunction::PositionInterval TransferFunction::_getInterval(Float position) const {
	assert(getNumberOfPoints() >= SizeType(2));
	assert(position >= Float(0.0f));
	assert(position <= Float(1.0f));

	auto it = _controlPoints.begin();

	for (; it != _controlPoints.end(); ++it) {
		if (it->getPosition() > position) { // find first that is bigger
			break;
		}
	}

	if (it == _controlPoints.end()) {
		it = std::prev(it); // point to the last (assuming we have at least 2 points)
	}

	return std::make_pair(std::prev(it), it);
}

Color TransferFunction::sample(Float position) const {
	position = Float(glm::clamp(position, Float(0.0f), Float(1.0f)));

	const auto interval = _getInterval(position);
	assert(interval.first->getPosition() <= interval.second->getPosition());

	if (interval.first->getPosition() == interval.second->getPosition()) {
		return interval.first->getColor();
	}

	return ayla::linearInterpolation(
		interval.first->getColor(),
		interval.second->getColor(),
		position,
		interval.first->getPosition(),
		interval.second->getPosition()
	);
}

std::vector<Color> TransferFunction::sampleAlongFunction(SizeType numberOfSteps) const {
	assert(numberOfSteps > SizeType(1));

	--numberOfSteps;
	std::vector<Color> samples;

	const auto stepSize = Float(1.0f) / Float(numberOfSteps);

	for (auto i = SizeType(0); i <= numberOfSteps; ++i) {
		samples.push_back(
			sample(i * stepSize)
		);
	}

	return samples;
}

SizeType TransferFunction::getNumberOfPoints() const {
	return SizeType(_controlPoints.size());
}

void TransferFunction::setPointPosition(SizeType pointIndex, Float newPosition) {
	assert(pointIndex < getNumberOfPoints());
	_controlPoints[pointIndex].setPosition(Float(glm::clamp(newPosition, 0.0f, 1.0f)));
}

void TransferFunction::setPointColor(SizeType pointIndex, const Color& newColor) {
	assert(pointIndex < getNumberOfPoints());
	_controlPoints[pointIndex].setColor(newColor);
}

Float TransferFunction::getPointPosition(SizeType pointIndex) const {
	assert(pointIndex < getNumberOfPoints());
	return _controlPoints[pointIndex].getPosition();
}

Color TransferFunction::getPointColor(SizeType pointIndex) const {
	assert(pointIndex < getNumberOfPoints());
	return _controlPoints[pointIndex].getColor();
}

glm::vec2 TransferFunction::getPointNormalized2DPosition(SizeType pointIndex) const {
	assert(pointIndex < getNumberOfPoints());
	return _controlPoints[pointIndex].getNormalized2DPosition();
}

void TransferFunction::setPointNormalized2DPosition(SizeType pointIndex, const glm::vec2& normalized2dPosition) {
	assert(pointIndex < getNumberOfPoints());

	Float minValue, maxValue;

	if (pointIndex == SizeType(0)) {
		minValue = maxValue = Float(0.0f);
	} else if (pointIndex == SizeType(getNumberOfPoints() - 1)) {
		minValue = maxValue = Float(1.0f);
	} else {
		minValue = getPointPosition(pointIndex - 1);
		maxValue = getPointPosition(pointIndex + 1);
	}
	
	const auto newPosition = glm::clamp(normalized2dPosition.x, minValue, maxValue);

	return _controlPoints[pointIndex].setNormalized2DPosition(
		glm::vec2(newPosition, normalized2dPosition.y)
	);
}

SizeType TransferFunction::addPoint(Float position, const Color& color) {
	position = Float(glm::clamp(position, Float(0.0f), Float(1.0f)));

	const auto positionInterval = _getInterval(position);
	const auto it = positionInterval.second;
	const auto newPointIndex = SizeType(it - _controlPoints.begin());

	_controlPoints.insert(it, ControlPoint(position, color));

	return newPointIndex;
}

bool TransferFunction::removePoint(SizeType pointIndex) {
	const auto canBeRemoved = (pointIndex > SizeType(0)) && (pointIndex < (getNumberOfPoints() - 1));

	if (canBeRemoved) {
		_controlPoints.erase(_controlPoints.begin() + pointIndex);
	}

	return canBeRemoved;
}

}