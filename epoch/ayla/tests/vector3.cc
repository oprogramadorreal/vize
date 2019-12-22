#include <boost/test/unit_test.hpp>
#include <boost/concept_check.hpp>

#include <ayla/geometry/vector.hpp>

#include <glm/gtx/vector_angle.hpp>

BOOST_AUTO_TEST_SUITE(ayla)
BOOST_AUTO_TEST_SUITE(vector3)

BOOST_AUTO_TEST_CASE( vec3_constructor3 ) {
	glm::vec3 testcase(2.0,3.0,5.0);

	BOOST_CHECK_EQUAL( testcase.x, 2.0 );
	BOOST_CHECK_EQUAL( testcase.y, 3.0 );
	BOOST_CHECK_EQUAL( testcase.z, 5.0 );
}

BOOST_AUTO_TEST_CASE( vec3_constructor1 ) {
	glm::vec3 testcase(7.0);

	BOOST_CHECK_EQUAL( testcase.x, 7.0 );
	BOOST_CHECK_EQUAL( testcase.y, 7.0 );
	BOOST_CHECK_EQUAL( testcase.z, 7.0 );
}

BOOST_AUTO_TEST_CASE( vec3_length ) {
	BOOST_CHECK_EQUAL( glm::length(glm::vec3(0,3,4)), 5 );
	BOOST_CHECK_EQUAL( glm::length(glm::vec3(0,4,3)), 5 );
	BOOST_CHECK_EQUAL( glm::length(glm::vec3(3,0,4)), 5 );
	BOOST_CHECK_EQUAL( glm::length(glm::vec3(4,0,3)), 5 );
	BOOST_CHECK_EQUAL( glm::length(glm::vec3(3,4,0)), 5 );
	BOOST_CHECK_EQUAL( glm::length(glm::vec3(4,3,0)), 5 );

	BOOST_CHECK_EQUAL( glm::length(glm::vec3(11,0,0)), 11 );
	BOOST_CHECK_EQUAL( glm::length(glm::vec3(0,13,0)), 13 );
	BOOST_CHECK_EQUAL( glm::length(glm::vec3(0,0,17)), 17 );
}

BOOST_AUTO_TEST_CASE( vec3_random_operators ) {
	BOOST_CHECK( (glm::vec3(1,0,0)+glm::vec3(1,0,0)) == (2.0f*glm::vec3(1,0,0)) );
	BOOST_CHECK( (glm::vec3(1,0,0)-glm::vec3(1,0,0)) == glm::vec3(0) );

	const Float epsilon = std::numeric_limits<Float>::epsilon();

	BOOST_CHECK(glm::epsilonEqual(glm::angle(glm::vec3(1,0,0), glm::vec3(0,1,0)), glm::radians(90.0f), epsilon));
	BOOST_CHECK(glm::epsilonEqual(glm::angle(glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)), glm::radians(90.0f), epsilon));

	BOOST_CHECK(glm::epsilonEqual(glm::angle(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)), glm::radians(90.0f), epsilon));
	BOOST_CHECK(glm::epsilonEqual(glm::angle(glm::vec3(1, 0, 0), glm::normalize(glm::vec3(1, 1, 0))), glm::radians(45.0f), epsilon));

	BOOST_CHECK_EQUAL( glm::dot(glm::vec3(1,0,0), glm::vec3(0,1,0) ), 0.0f );

	glm::vec3 one(1,1,1);
	one *= 3.0;
	BOOST_CHECK_EQUAL( one, glm::vec3(3,3,3) );
}

BOOST_AUTO_TEST_CASE( vec3_normalization ) {
	glm::vec3 testcase(3,4,0);
	BOOST_CHECK_EQUAL( glm::normalize(testcase), glm::vec3(3.0/5.0,4.0/5.0,0.0) );
	testcase = glm::normalize(testcase);
	BOOST_CHECK_EQUAL( testcase, glm::vec3(3.0/5.0,4.0/5.0,0.0) );
	
	//It is a feature
	glm::vec3 zero = glm::vec3(0);
	BOOST_CHECK( glm::normalize(zero) != glm::vec3(0) );
	zero = glm::normalize(zero);
	BOOST_CHECK( zero != glm::vec3(0) );
}

BOOST_AUTO_TEST_CASE( vec3_sscanfed ) {
	std::string input("v 1.0 2.0 3.0 n 5.0 7.0 11.0 u 13.14 14.15");
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec3 uv = glm::vec3(0);
	sscanf(input.c_str(), "v %f %f %f n %f %f %f u %f %f", &vertex.x, &vertex.y, &vertex.z, &normal.x, &normal.y, &normal.z, &uv.x, &uv.y );

	BOOST_CHECK( vertex == glm::vec3(1.0,2.0,3.0) );
	BOOST_CHECK( normal == glm::vec3(5.0,7.0,11.0) );
	BOOST_CHECK( uv == glm::vec3(13.14f,14.15f, 0.0f) );
}

BOOST_AUTO_TEST_CASE( vec3_obb_sscanfed ) {
	std::string aux("obb c[-1.04700 -1.12000 0.66100] e[0.30300 0.18200 0.35300] ux[1.00000 0.00000 0.00000] uy[0.00000 1.00000 0.00000] uz[0.00000 0.00000 1.00000]");
	glm::vec3 c, e, ux, uy, uz;
	sscanf(aux.c_str(), "obb c[%f %f %f] e[%f %f %f] ux[%f %f %f] uy[%f %f %f] uz[%f %f %f]",  &c.x, &c.y, &c.z, &e.x, &e.y, &e.z, &ux.x, &ux.y, &ux.z, &uy.x, &uy.y, &uy.z, &uz.x, &uz.y, &uz.z);
	BOOST_CHECK_EQUAL( c, glm::vec3(-1.04700,-1.12000,0.66100) );
	BOOST_CHECK_EQUAL( e, glm::vec3(0.30300,0.18200,0.35300) );
	BOOST_CHECK_EQUAL( ux, glm::vec3(1,0,0) );
	BOOST_CHECK_EQUAL( uy, glm::vec3(0,1,0) );
	BOOST_CHECK_EQUAL( uz, glm::vec3(0,0,1) );
}

BOOST_AUTO_TEST_CASE( simple_cross_product ) {
	BOOST_CHECK_EQUAL( glm::cross(glm::vec3(1,0,0), glm::vec3(0,1,0)), glm::vec3(0,0,1) );
}

BOOST_AUTO_TEST_CASE(orthogonal_test) {
	for (SizeType i = 0u; i < 10u; ++i) {
		const glm::vec3 a(-100 + rand() % 200, -100 + rand() % 200, -100 + rand() % 200);
		const glm::vec3 ortho = orthogonal(a);
		BOOST_CHECK( !ayla::isZero(glm::length(ortho)) );
		BOOST_CHECK_CLOSE(glm::angle(a, ortho), glm::radians(90.0f), 0.0001f);
	}

	const glm::vec3 x(1.0f, 0.0f, 0.0f);
	const glm::vec3 orthoX = orthogonal(x);
	BOOST_CHECK( !ayla::isZero(glm::length(orthoX)) );
	BOOST_CHECK_CLOSE(glm::angle(x, orthoX), glm::radians(90.0f), 0.0001f);
	
	const glm::vec3 y(0.0f, 1.0f, 0.0f);
	const glm::vec3 orthoY = orthogonal(y);
	BOOST_CHECK( !ayla::isZero(glm::length(orthoY)) );
	BOOST_CHECK_CLOSE(glm::angle(y, orthoY), glm::radians(90.0f), 0.0001f);
	
	const glm::vec3 z(0.0f, 0.0f, 1.0f);
	const glm::vec3 orthoZ = orthogonal(z);
	BOOST_CHECK( !ayla::isZero(glm::length(orthoZ)) );
	BOOST_CHECK_CLOSE(glm::angle(z, orthoZ), glm::radians(90.0f), 0.0001f);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

