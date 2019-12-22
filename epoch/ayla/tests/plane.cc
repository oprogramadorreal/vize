#include <boost/test/unit_test.hpp>
#include <ayla/geometry/plane.hpp>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>
#include <ayla/geometry/vector.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE( ayla )
BOOST_AUTO_TEST_SUITE( _plane )

BOOST_AUTO_TEST_CASE( plane_projectVector ) {
	ayla::Plane planeXY( glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f) );
	
	for (int i = -10; i < 10; ++i) {
		glm::vec3 projected = planeXY.projectVector( glm::vec3(1.0f, 1.0f, i) );
		BOOST_CHECK( projected == glm::vec3(1.0f, 1.0f, 0.0f) );
	}
	
	ayla::Plane planeXZ( glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f) );
	
	for (int i = -10; i < 10; ++i) {
		glm::vec3 projected = planeXZ.projectVector( glm::vec3(1.0f, i, 1.0f) );
		BOOST_CHECK( projected == glm::vec3(1.0f, 0.0f, 1.0f) );
	}
	
	ayla::Plane planeYZ( glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) );
	
	for (int i = -10; i < 10; ++i) {
		glm::vec3 projected = planeYZ.projectVector( glm::vec3(i, 1.0f, 1.0f) );
		BOOST_CHECK( projected == glm::vec3(0.0f, 1.0f, 1.0f) );
	}
	
	ayla::Plane notOnOrigin( glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(3.0f, 10.0f, 10.0f) );
	
	for (int i = -10; i < 10; ++i) {
		glm::vec3 projected = notOnOrigin.projectVector( glm::vec3(1.0f, 1.0f, i) );
		BOOST_CHECK( projected == glm::vec3(1.0f, 1.0f, 0.0f) );
	}
	
	ayla::Plane inclined( glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)), glm::vec3(10.0f, 5.0f, 3.0f) );
	
	for (int i = -10; i < 10; i++) {
		glm::vec3 vector = glm::vec3(1.0f, 0.0f, 0.0) + Float(i) * glm::vec3(0.0f, 1.0f, 1.0f);
		glm::vec3 projected = inclined.projectVector(vector);
		
		BOOST_CHECK( ayla::epsilonEqual( projected, glm::vec3(1.0f, 0.0f, 0.0f), AYLA_PLANE_DEFAULT_EPSILON ) );
	}
}

BOOST_AUTO_TEST_CASE( plane_projectPoint ) {
	ayla::Plane planeXY( glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f) );
	
	for (unsigned i = -10; i < 10; ++i) {
		glm::vec3 projected = planeXY.projectPoint( glm::vec3(1.0f, 1.0f, i) );
		BOOST_CHECK( projected == glm::vec3(1.0f, 1.0f, 0.0f) );
	}
	
	ayla::Plane planeXZ( glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f) );
	
	for (unsigned i = -10; i < 10; ++i) {
		glm::vec3 projected = planeXZ.projectPoint( glm::vec3(1.0f, i, 1.0f) );
		BOOST_CHECK( projected == glm::vec3(1.0f, 0.0f, 1.0f) );
	}
	
	ayla::Plane planeYZ( glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) );
	
	for (unsigned i = -10; i < 10; ++i) {
		glm::vec3 projected = planeYZ.projectPoint( glm::vec3(i, 1.0f, 1.0f) );
		BOOST_CHECK( projected == glm::vec3(0.0f, 1.0f, 1.0f) );
	}
	
	ayla::Plane notOnOrigin( glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(3.0f, 10.0f, 10.0f) );
	
	for (unsigned i = -10; i < 10; ++i) {
		glm::vec3 projected = notOnOrigin.projectPoint( glm::vec3(1.0f, 1.0f, i) );
		BOOST_CHECK( projected == glm::vec3(1.0f, 1.0f, 10.0f) );
	}
	
	ayla::Plane inclined( glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)), glm::vec3(1.0f, 5.0f, 3.0f) );
	
	for (int i = -10; i < 10; i++) {
		glm::vec3 vector = glm::vec3(1.0f, 2.0f, 0.0) + Float(i) * glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f));
		glm::vec3 projected = inclined.projectPoint(vector);
		BOOST_CHECK( ayla::epsilonEqual( projected, glm::vec3(1.0f, 5.0f, 3.0f), AYLA_PLANE_DEFAULT_EPSILON ) );
	}
}

BOOST_AUTO_TEST_CASE( plane_belongs ) {
	ayla::Plane planeXY( glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f) );
	
	BOOST_CHECK( planeXY.belongs(planeXY.getOrigin()) );
	BOOST_CHECK( planeXY.belongs(glm::vec3(0.0f)) );
	
	for (unsigned i = 0; i < 360; ++i) {
		Float t = glm::pi<Float>() * Float(i)/180.0f;
		glm::vec3 vector( glm::sin(t), glm::cos(t), 0.0f);
		BOOST_CHECK( planeXY.belongs(vector) );
	}
}



BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
