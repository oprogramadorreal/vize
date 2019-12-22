#include <boost/test/unit_test.hpp>
#include <ayla/geometry/vector.hpp>

#include <glm/gtc/epsilon.hpp>

BOOST_AUTO_TEST_SUITE( ayla )
BOOST_AUTO_TEST_SUITE( _float )

BOOST_AUTO_TEST_CASE( less_than_zero ) {
	const Float a(0);
	BOOST_CHECK(!(a < 0.0f));
	BOOST_CHECK(a <= 0.0f);
}

BOOST_AUTO_TEST_CASE(_isZero) {
	const Float a(0.0f);

	BOOST_CHECK(isZero(a));

	BOOST_CHECK(isZero(std::numeric_limits<Float>::epsilon()));
	BOOST_CHECK(!isZero(2 * std::numeric_limits<Float>::epsilon()));
}

BOOST_AUTO_TEST_CASE(_epsilonEqual) {
	const Float epsilon = std::numeric_limits<Float>::epsilon();

	const Float a = 1.0f;
	const Float b = a + epsilon;
	
	BOOST_CHECK(glm::epsilonEqual(a, b, epsilon * 2.0f));
	BOOST_CHECK(glm::epsilonNotEqual(a, b, epsilon));

	const Float c = b + epsilon;

	BOOST_CHECK(glm::epsilonNotEqual(a, c, epsilon));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

