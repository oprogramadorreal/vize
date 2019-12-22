#include <boost/test/unit_test.hpp>

#include <ayla/geometry/vector.hpp>
#include <ayla/geometry/matrix.hpp>
#include <ayla/geometry/rigid_transform.hpp>

#include <glm/gtc/matrix_transform.hpp>

BOOST_AUTO_TEST_SUITE(ayla)
BOOST_AUTO_TEST_SUITE(matrix4)

BOOST_AUTO_TEST_CASE( mat4_constructor16 ) {
	glm::mat4 testcase(1.0f,2.0f,3.0f,4.0f,
			5.0f,6.0f,7.0f,8.0f,
			9.0f,10.0f,11.0f,12.0f,
			13.0f,14.0f,15.0f,16.0f);

	Float count=1.0f;
	for (SizeType row = 0; row < 4; ++row) {
		for (SizeType col = 0; col < 4; ++col) {
			BOOST_CHECK_EQUAL(testcase[row][col], count);
			count += 1.0;
		}
	}
}

BOOST_AUTO_TEST_CASE( mat4_scale_transformation_and_inverse ) {
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 3.0, 5.0));
	glm::vec4 point(1.0,1.0,1.0,1.0);

	BOOST_CHECK_EQUAL( (scale*point).x, 2.0 );
	BOOST_CHECK_EQUAL( (scale*point).y, 3.0 );
	BOOST_CHECK_EQUAL( (scale*point).z, 5.0 );
	BOOST_CHECK_EQUAL( (scale*point).w, 1.0 );

	point = scale*point;

	BOOST_CHECK_EQUAL( (scale*point).x, 4.0 );
	BOOST_CHECK_EQUAL( (scale*point).y, 9.0 );
	BOOST_CHECK_EQUAL( (scale*point).z, 25.0 );
	BOOST_CHECK_EQUAL( (scale*point).w, 1.0 );

	point = scale*point;

	glm::mat4 scaleInv = glm::inverse(scale);
	scale = scaleInv;

	point = scaleInv*point;
	point = scale*point;

	BOOST_CHECK( glm::epsilonEqual(point.x, 1.0f, 0.00001f) );
	BOOST_CHECK( glm::epsilonEqual(point.y, 1.0f, 0.00001f) );
	BOOST_CHECK( glm::epsilonEqual(point.z, 1.0f, 0.00001f) );
	BOOST_CHECK( glm::epsilonEqual(point.w, 1.0f, 0.00001f) );

	const glm::mat4 identity = scale*glm::inverse(scaleInv);
	BOOST_CHECK( epsilonEqual(identity, glm::mat4(1.0f), 0.00001f) );
}

BOOST_AUTO_TEST_CASE( mat4_constructor_from_rigid_transform_translate ) {
	RigidTransform t = RigidTransform::getIdentity();
	t.translate(glm::vec3(3.0,5.0,7.0));

	glm::mat4 mat4= t.toMatrix4();

	BOOST_CHECK_EQUAL( mat4,
		glm::mat4(
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			3,5,7,1
		)
	);
}

BOOST_AUTO_TEST_CASE( mat4_constructor_from_rigid_transform_ytranslate ) {
	for (SizeType i = 0; i < 360; ++i) {
		RigidTransform t = RigidTransform::getIdentity();
		const Float rotationAmount = glm::radians(Float(i));
		t.rotate(rotationAmount,glm::vec3(0,1,0));
		const glm::mat4 fromRigidTransform = t.toMatrix4();
		const glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), rotationAmount, glm::vec3(0,1,0));

		BOOST_CHECK_MESSAGE( epsilonEqual(fromRigidTransform, rotateY,0.000001f), fromRigidTransform << " != " << rotateY);
	}
}

BOOST_AUTO_TEST_CASE( mat4_constructor_from_rigid_transform_with_rotation_and_translation ) {
	for (SizeType i = 0; i < 1; ++i) {
		RigidTransform t = RigidTransform::getIdentity();
		const Float rotationAmount = glm::radians(Float(i));
		const glm::vec3 translateAmount(glm::vec3(Float(3*i),Float(5*i),Float(7*i)));
		t.rotate(rotationAmount,glm::vec3(0,1,0));
		t.translate(translateAmount);

		const glm::mat4 fromRigidTransform = t.toMatrix4();

		const glm::mat4 fromMatrix = glm::rotate(
			glm::translate(glm::mat4(1.0f), translateAmount),
			rotationAmount,
			glm::vec3(0,1,0)
		);

		BOOST_CHECK_MESSAGE( epsilonEqual(fromRigidTransform, fromMatrix, 0.000001f), fromRigidTransform << " != " << fromMatrix);
	}
}

BOOST_AUTO_TEST_CASE( mat4_constructor_from_rigid_transform_lot_of_rotations_then_translate ) {
	RigidTransform t = RigidTransform::getIdentity();
	const Float rotationAmountA = glm::radians(Float(13));
	const Float rotationAmountB = glm::radians(Float(17));

	for (SizeType i = 0; i < 5; ++i) {
		t.rotate(rotationAmountA,glm::vec3(0,1,0));
		t.translate(glm::vec3(2,3,5));
		t.rotate(rotationAmountB,glm::vec3(1,0,0));
	}
	glm::mat4 fromRigidTransform = t.toMatrix4();

	glm::mat4 fromMatrix = glm::mat4(1.0f);
	for (SizeType i = 0; i < 5; ++i) {
		fromMatrix = glm::rotate(glm::mat4(1.0f), rotationAmountA, glm::vec3(0,1,0)) * fromMatrix; // glm::mat4::rotateY(rotationAmountA)*fromMatrix;
		fromMatrix = glm::rotate(glm::mat4(1.0f), rotationAmountB, glm::vec3(1,0,0)) * fromMatrix; //glm::mat4::rotateX(rotationAmountB)*fromMatrix;
	}
	fromMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2*5,3*5,5*5))*fromMatrix;

	//High tolerance to errors because matrix accumulate a lot of numerical instability
	BOOST_CHECK_MESSAGE( epsilonEqual(fromRigidTransform, fromMatrix, 0.1f), fromRigidTransform << " != " << fromMatrix);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
