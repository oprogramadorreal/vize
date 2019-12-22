#include <boost/test/unit_test.hpp>

#include <ayla/geometry/triangle.hpp>
#include <ayla/geometry/vector.hpp>

BOOST_AUTO_TEST_SUITE(ayla)
BOOST_AUTO_TEST_SUITE(triangle)

BOOST_AUTO_TEST_CASE( basicTest ) {
	Triangle t0(glm::vec3(-11, 0, 0), glm::vec3(0, 11, 0), glm::vec3(22,-11, 0));
	
	BOOST_CHECK(glm::epsilonEqual(t0.calculatePerimeter(), 81.4541f, 0.001f));
	BOOST_CHECK(glm::epsilonEqual(t0.calculateArea(), 242.0f, 0.001f));
	
	BOOST_CHECK(epsilonEqual(t0.getNormal(), -glm::vec3(0,0,1), 0.0000001f));
}

BOOST_AUTO_TEST_CASE( barymetric_coordinates_coplanar ) {
	glm::vec3 A(-3, -4, 0);
	glm::vec3 B(0, 5, 0);
	glm::vec3 C(3, -4, 0);
	Triangle t0(A,B,C);
	
	glm::vec3 P(0.0f,0.0f,0.0f);
	Float a, b, c;
	t0.getBarycentricCoordinates(P, a, b, c);
	glm::vec3 result = a*A + b*B + c*C;
	
	BOOST_CHECK(isZero(result.x));
	BOOST_CHECK(isZero(result.y));
	BOOST_CHECK(isZero(result.z));
	
	glm::vec3 halfEdge = (B-A);
	halfEdge /= 2;
	P = A + halfEdge;
	t0.getBarycentricCoordinates(P, a, b, c);
	
	BOOST_CHECK_CLOSE( a, 1/2.0f, 0.00001f );
	BOOST_CHECK_CLOSE( b, 1/2.0f, 0.00001f );
	BOOST_CHECK_CLOSE( c, 0.0f, 0.00001f );	
}

BOOST_AUTO_TEST_CASE( closest_point_coplanar ) {
	glm::vec3 A(-3, -4, 0);
	glm::vec3 B(0, 5, 0);
	glm::vec3 C(3, -4, 0);
	Triangle t0(A,B,C);
	Float a, b, c;
	
	//Point is inside the triangle
	glm::vec3 P(0,0,0);
	glm::vec3 closest = t0.getClosestPoint(P, a, b, c);
	
	glm::vec3 result = A*a + B*b + C*c;
	
	BOOST_CHECK(isZero(closest.x));
	BOOST_CHECK(isZero(closest.y));
	BOOST_CHECK(isZero(closest.z));
	BOOST_CHECK(isZero(result.x));
	BOOST_CHECK(isZero(result.y));
	BOOST_CHECK(isZero(result.z));
	
	//Point outside the triangle, closest to one vertex
	P = glm::vec3( 0, 10, 0 );
	closest = t0.getClosestPoint(P, a, b, c);
	result = A*a + B*b + C*c;
	
	//Result and Closest should be equal to B
	BOOST_CHECK_CLOSE( closest.x, B.x, 0.0001f );
	BOOST_CHECK_CLOSE( closest.y, B.y, 0.0001f );
	BOOST_CHECK_CLOSE( closest.z, B.z, 0.0001f );
	BOOST_CHECK_CLOSE( result.x, B.x, 0.0001f );
	BOOST_CHECK_CLOSE( result.y, B.y, 0.0001f );
	BOOST_CHECK_CLOSE( result.z, B.z, 0.0001f );
	
	//Point outside the triangle, colinear with the middle of the edge AB and the point (0,0,0).
	//The closes point should be the half of the edge.
	glm::vec3 halfEdge = (B-A);
	halfEdge /= 2.0f;
	halfEdge = A + halfEdge;
	P = halfEdge + halfEdge;
	closest = t0.getClosestPoint(P, a, b, c);
	result = A*a + B*b + C*c;
	
	//Result and Closest should be equal to halfEdge
	BOOST_CHECK_CLOSE( closest.x, halfEdge.x, 0.0001f );
	BOOST_CHECK_CLOSE( closest.y, halfEdge.y, 0.0001f );
	BOOST_CHECK_CLOSE( closest.z, halfEdge.z, 0.0001f );
	BOOST_CHECK_CLOSE( result.x, halfEdge.x, 0.0001f );
	BOOST_CHECK_CLOSE( result.y, halfEdge.y, 0.0001f );
	BOOST_CHECK_CLOSE( result.z, halfEdge.z, 0.0001f );
}

BOOST_AUTO_TEST_CASE( closest_point_not_coplanar ) {
	glm::vec3 A(-3, -4, 0);
	glm::vec3 B(0, 5, 0);
	glm::vec3 C(3, -4, 0);
	Triangle t0(A,B,C);
	Float a, b, c;
	
	//Point is "over" the triangle
	//Should be mapped to (0,0,0)
	glm::vec3 P(0,0,20);
	glm::vec3 closest = t0.getClosestPoint(P, a, b, c);
	
	glm::vec3 result = A*a + B*b + C*c;
	
	BOOST_CHECK(isZero(closest.x));
	BOOST_CHECK(isZero(closest.y));
	BOOST_CHECK(isZero(closest.z));
	BOOST_CHECK(isZero(result.x));
	BOOST_CHECK(isZero(result.y));
	BOOST_CHECK(isZero(result.z));
	
	//Point outside the triangle, closest to one vertex
	P = glm::vec3( 0, 10, 7 );
	closest = t0.getClosestPoint(P, a, b, c);
	result = A*a + B*b + C*c;
	
	//Result and Closest should be equal to B
	BOOST_CHECK_CLOSE( closest.x, B.x, 0.0001f );
	BOOST_CHECK_CLOSE( closest.y, B.y, 0.0001f );
	BOOST_CHECK_CLOSE( closest.z, B.z, 0.0001f );
	BOOST_CHECK_CLOSE( result.x, B.x, 0.0001f );
	BOOST_CHECK_CLOSE( result.y, B.y, 0.0001f );
	BOOST_CHECK_CLOSE( result.z, B.z, 0.0001f );
	
	//Point outside the triangle, colinear with the middle of the edge AB and the point (0,0,0).
	//The closes point should be the half of the edge.
	glm::vec3 halfEdge = (B-A);
	halfEdge /= 2.0f;
	halfEdge = A + halfEdge;
	P = halfEdge + halfEdge + glm::vec3(0,0,5);
	closest = t0.getClosestPoint(P, a, b, c);
	result = A*a + B*b + C*c;
	
	//Result and Closest should be equal to halfEdge
	BOOST_CHECK_CLOSE( closest.x, halfEdge.x, 0.0001f );
	BOOST_CHECK_CLOSE( closest.y, halfEdge.y, 0.0001f );
	BOOST_CHECK_CLOSE( closest.z, halfEdge.z, 0.0001f );
	BOOST_CHECK_CLOSE( result.x, halfEdge.x, 0.0001f );
	BOOST_CHECK_CLOSE( result.y, halfEdge.y, 0.0001f );
	BOOST_CHECK_CLOSE( result.z, halfEdge.z, 0.0001f );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
