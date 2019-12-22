#include <boost/test/unit_test.hpp>

#include <ayla/geometry/vector.hpp>
#include <ayla/geometry/matrix.hpp>

#include <glm/gtc/matrix_access.hpp>

BOOST_AUTO_TEST_SUITE( ayla )
BOOST_AUTO_TEST_SUITE( matrix3 )

BOOST_AUTO_TEST_CASE( mat3_constructor1 ) {
	glm::mat3 oneValue(13.0f);

	for (SizeType row = 0; row < 3; ++row) {
		for (SizeType col = 0; col < 3; ++col) {
			if (row == col) {
				BOOST_CHECK_EQUAL(oneValue[row][col], 13.0f);
			} else {
				BOOST_CHECK_EQUAL(oneValue[row][col], 0.0f);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE( mat3_constructor_col_vecs ) {
	glm::mat3 testcase( glm::vec3(1.0,2.0,3.0), glm::vec3(4.0,5.0,6.0), glm::vec3(7.0,8.0,9.0) );//Col constructor

	Float count = 1.0;
	for (SizeType col = 0; col < 3; ++col) {
		for (SizeType row = 0; row < 3; ++row) {
			BOOST_CHECK_EQUAL(testcase[col][row], count);
			count += 1;
		}
	}
}

BOOST_AUTO_TEST_CASE( mat3_constructor9 ) {
	glm::mat3 testcase( 9.0,8.0,7.0,
			6.0,5.0,4.0,
			3.0,2.0,1.0);

	Float count = 9.0;
	for (SizeType row = 0; row < 3; ++row) {
		for (SizeType col = 0; col < 3; ++col) {
			BOOST_CHECK_EQUAL(testcase[row][col], count);
			count -= 1;
		}
	}
}

BOOST_AUTO_TEST_CASE( vec_multiplication_and_invert ) {
	glm::mat3 scale(2.0,0.0,0.0,
		      0.0,3.0,0.0,
		      0.0,0.0,5.0);
	
	glm::vec3 point(1.0,1.0,1.0);

	BOOST_CHECK_EQUAL( (scale*point).x, 2.0 );
	point = scale*point;
	BOOST_CHECK_EQUAL( point.y, 3.0 );
	point = scale*point;
	BOOST_CHECK_EQUAL( point.z, 25.0 );

	glm::mat3 scaleInv = glm::inverse(scale);
	glm::mat3 scaleOriginal = scale;
	scale = glm::inverse(scale);

	point = scaleInv*point;
	point = scale*point;

	BOOST_CHECK( glm::epsilonEqual(point.x, 1.0f, 0.00001f) );
	BOOST_CHECK( glm::epsilonEqual(point.y, 1.0f, 0.00001f) );
	BOOST_CHECK( glm::epsilonEqual(point.z, 1.0f, 0.00001f) );

	glm::mat3 identity = scaleOriginal*scaleInv;
	BOOST_CHECK(ayla::epsilonEqual(identity, glm::mat3(1.0f), 0.00001f));
}

BOOST_AUTO_TEST_CASE( random_tests_on_operators ) {
	BOOST_CHECK((glm::mat3(1.0f) + glm::mat3(1.0f)) == 2.0f * glm::mat3(1.0f));
	BOOST_CHECK( (glm::mat3(1.0f) - glm::mat3(1.0f)) == glm::mat3(0.0f) );
}

BOOST_AUTO_TEST_CASE( outerProductTest ) {
	glm::vec3 a(1,-2,3), b(-9,8,7);

	glm::mat3 m0 = glm::outerProduct(a, b);
	
	glm::mat3 r(
		-9, 18, -27,
		8, -16, 24,
		7, -14, 21
	);
		
	BOOST_CHECK(m0 == r);
}

BOOST_AUTO_TEST_CASE( transposeTest ) {
	glm::mat3 m0(
		3, 4, 5,
		-1, 0, 1,
		4, 11, -10
	);
	
	glm::mat3 m1 = glm::transpose(m0);
	
	const glm::mat3 m2 = glm::transpose(m0);

	BOOST_CHECK(m1 == m2);
	BOOST_CHECK(m0 == glm::transpose(m1));
	BOOST_CHECK(m0 == glm::transpose(m2));
	
	glm::mat3 m3(
		3, -1, 4,
		4, 0, 11,
		5, 1, -10
	);
	
	BOOST_CHECK(m3 == m2);
	BOOST_CHECK(m3 == m1);
}

BOOST_AUTO_TEST_CASE(row_column_access) {
	const glm::mat3 m1(
		1, 4, 7, // column 0
		2, 5, 8, // column 1
		3, 6, 9 // column 2
	);

	const glm::mat3 m2(
		1, 2, 3, // column 0
		4, 5, 6, // column 1
		7, 8, 9 // column 2
	);

	BOOST_CHECK(glm::row(m1, 0) == glm::column(m2, 0));
	BOOST_CHECK(glm::row(m1, 1) == glm::column(m2, 1));
	BOOST_CHECK(glm::row(m1, 2) == glm::column(m2, 2));

	BOOST_CHECK(glm::column(m1, 0) == glm::row(m2, 0));
	BOOST_CHECK(glm::column(m1, 1) == glm::row(m2, 1));
	BOOST_CHECK(glm::column(m1, 2) == glm::row(m2, 2));

	BOOST_CHECK(glm::row(m2, 2) == glm::vec3(3,6,9));
	BOOST_CHECK(glm::column(m2, 0) == glm::vec3(1,2,3));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
