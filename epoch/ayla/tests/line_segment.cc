#include <boost/test/unit_test.hpp>

#include <ayla/geometry/line_segment.hpp>
#include <ayla/geometry/vector.hpp>

#include <iostream>
#include <stdio.h>

BOOST_AUTO_TEST_SUITE(ayla)
BOOST_AUTO_TEST_SUITE(line_segment)

BOOST_AUTO_TEST_CASE( lengthTest ) {
	const LineSegment line(glm::vec3(-10), glm::vec3(10));
	const Float len = 20.0f * glm::sqrt(3.0f);
	const Float epsilon = 0.0005f;

	BOOST_CHECK(glm::epsilonEqual(line.length(), len, epsilon));
	BOOST_CHECK(glm::epsilonEqual(line.length2(), len*len, epsilon));
}

BOOST_AUTO_TEST_CASE( getClosestPointTest ) {
	const glm::vec3 p1(-10, -10, 0);
	const glm::vec3 p2(10, -10, 0);

	const LineSegment line(p1, p2);

	BOOST_CHECK(ayla::epsilonEqual(line.getClosestPoint(glm::vec3(0, 1, 0)), glm::vec3(0, -10, 0), 0.0000001f));

	BOOST_CHECK(ayla::epsilonEqual(line.getClosestPoint(glm::vec3(100, 100, 0)), p2, 0.0000001f));
	BOOST_CHECK(ayla::epsilonEqual(line.getClosestPoint(glm::vec3(-100, -100, 0)), p1, 0.0000001f));
}

BOOST_AUTO_TEST_CASE( intersectionTest ) {
	const LineSegment l0(glm::vec3(-10,-10,0), glm::vec3(-10,10,0));
	LineSegment l1(glm::vec3(0,0,0), glm::vec3(0,10,0));

	glm::vec3 pt;
	BOOST_CHECK(l0.intersects(l1, pt) == false);

	l1 = LineSegment(glm::vec3(0,0,0), glm::vec3(-5,0,0));
	BOOST_CHECK(l0.intersects(l1, pt) == false);

	l1 = LineSegment(glm::vec3(0,0,0), glm::vec3(-20,0,0));
	BOOST_CHECK(l0.intersects(l1, pt) == true);
	BOOST_CHECK(ayla::epsilonEqual(pt, glm::vec3(-10, 0, 0), 0.0000001f));

	l1 = LineSegment(glm::vec3(-20,-10,0), glm::vec3(20,20,0));
	BOOST_CHECK(l0.intersects(l1, pt) == true);
	BOOST_CHECK(ayla::epsilonEqual(pt, glm::vec3(-10, -2.5f, 0), 0.0000001f));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
