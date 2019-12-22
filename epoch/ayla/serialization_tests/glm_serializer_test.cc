#include <boost/test/unit_test.hpp>

#include <ayla/serialization/glm_serializer.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

BOOST_AUTO_TEST_SUITE(ayla)
BOOST_AUTO_TEST_SUITE(glm_serializer)

BOOST_AUTO_TEST_CASE(serializeVec3) {
    glm::vec3 expectedVector(10, 20, 30), actualVector;

    std::stringstream stream;

	{
		boost::archive::xml_oarchive oa(stream);
		oa& BOOST_SERIALIZATION_NVP(expectedVector);
	}

	{
		boost::archive::xml_iarchive ia(stream);
		ia& BOOST_SERIALIZATION_NVP(actualVector);
	}

    BOOST_CHECK_EQUAL(expectedVector.x, actualVector.x);
    BOOST_CHECK_EQUAL(expectedVector.y, actualVector.y);
    BOOST_CHECK_EQUAL(expectedVector.z, actualVector.z);
}

BOOST_AUTO_TEST_CASE(serializeVec2) {
    glm::vec2 expectedVector(12, -12), actualVector;

    std::stringstream stream;

	{
		boost::archive::xml_oarchive oa(stream);
		oa& BOOST_SERIALIZATION_NVP(expectedVector);
	}

	{
		boost::archive::xml_iarchive ia(stream);
		ia& BOOST_SERIALIZATION_NVP(actualVector);
	}

    BOOST_CHECK_EQUAL(expectedVector.x, actualVector.x);
    BOOST_CHECK_EQUAL(expectedVector.y, actualVector.y);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()