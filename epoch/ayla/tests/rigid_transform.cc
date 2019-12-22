#include <boost/test/unit_test.hpp>

#include <ayla/geometry/rigid_transform.hpp>
#include <ayla/geometry/vector.hpp>
#include <ayla/geometry/quaternion.hpp>

#include <glm/gtc/epsilon.hpp>

BOOST_AUTO_TEST_SUITE(ayla)
BOOST_AUTO_TEST_SUITE( rigid_transform )

BOOST_AUTO_TEST_CASE( rigid_transform_constructors ) {
	const glm::quat quat = glm::angleAxis(glm::radians(30.0f), glm::vec3(0,1,0));
	glm::vec3 vec(11.0,13.0,17.0);

	RigidTransform transform(vec,quat);
	BOOST_CHECK_EQUAL( transform.getTranslation(), vec );
	BOOST_CHECK_EQUAL( transform.getRotation(), quat );
}

BOOST_AUTO_TEST_CASE( rigid_transform_inverses ) {
	//Rigid transform inverse depends on quaternion * vector, the following case seemed to fail
	BOOST_CHECK_EQUAL( glm::quat(0.819152f,0,0.573576f,0)*glm::vec3(0,0,-15), glm::vec3(-14.0953769683837890625f, 0.0f, -5.13031768798828125f) );

	const auto inv = RigidTransform(glm::vec3(0,0,15),glm::quat(0.819152f,0,0.573576f,0)).getInverse();

	const Float episilon = 0.000001f;

	BOOST_CHECK(glm::epsilonEqual(inv.getTranslation().x, 14.09539508819580078125f, episilon));
	BOOST_CHECK(glm::epsilonEqual(inv.getTranslation().y, 0.0f, episilon));
	BOOST_CHECK(glm::epsilonEqual(inv.getTranslation().z, -5.13030529022216796875f, episilon));

	BOOST_CHECK(glm::epsilonEqual(inv.getRotation().w, 0.81915247440338134765625f, episilon));
	BOOST_CHECK(glm::epsilonEqual(inv.getRotation().x, 0.0f, episilon));
	BOOST_CHECK(glm::epsilonEqual(inv.getRotation().y, -0.57357633113861083984375f, episilon));
	BOOST_CHECK(glm::epsilonEqual(inv.getRotation().z, 0.0f, episilon));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
