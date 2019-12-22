#include <ayla/geometry/intersection.hpp>
#include <ayla/geometry/vector.hpp>

#include <glm/gtx/rotate_vector.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( ayla )
BOOST_AUTO_TEST_SUITE( intersection ) 

BOOST_AUTO_TEST_CASE( ray_aab_intersection ) {
	AxisAlignedBox aab( glm::vec3( 0.0, 0.0, 0.0 ), 5.0, 5.0, 5.0 );
	Float d;
	
	for (int i = -5; i < 5; i++) {
		Ray ray( glm::vec3( 0.0, 0.0, 0.0 ), glm::normalize(glm::vec3( i, 1.0, 0.0 )) );
		BOOST_CHECK( ayla::intersection::rayAab( ray, aab, d ) );
	}
	
	for (int i = -5; i < 5; i++) {
		Ray ray(glm::vec3(0.0, 5.1, 0.0), glm::normalize(glm::vec3(i, 1.0, 0.0)));
		BOOST_CHECK( ! ayla::intersection::rayAab( ray, aab, d ) );
	}

	Ray rayA( glm::vec3( 0.0, 6.0, 0.0 ), glm::vec3( 1.0, 0.0, 0.0)  );
	BOOST_CHECK( ! ayla::intersection::rayAab( rayA, aab, d ) );
	
	Ray rayB(glm::vec3(0.0, 6.0, 0.0), glm::normalize(glm::vec3(1.0, -1.0, 0.0)));
	BOOST_CHECK( ayla::intersection::rayAab( rayB, aab, d ) );
	
	Ray rayC(glm::vec3(10.0, 10.0, 10.0), glm::normalize(glm::vec3(-1.0, -1.0, -1.0)));
	BOOST_CHECK(  ayla::intersection::rayAab( rayC, aab, d ) );
}

BOOST_AUTO_TEST_CASE( ray_box_intersection ) { 
	Box box(glm::vec3(0.0, 0.0, 0.0), 5.0, 5.0, 5.0);
	Float d;
	
	RigidTransform transform = RigidTransform::getIdentity();
	
	// Testing first when the Box is aligned. Then rotating 90 degrees around the Y axis.
	for (int k = 0; k < 3; k++) {
		for (int i = -5; i < 5; i++) {
			Ray ray(glm::vec3(0.0, 0.0, 0.0), glm::normalize(glm::vec3(i, 1.0, 0.0)));
			BOOST_CHECK( ayla::intersection::rayBox( ray, box, d ) );
		}

		for (int i = -5; i < 5; i++) {
			Ray ray(glm::vec3(0.0, 5.1, 0.0), glm::normalize(glm::vec3(i, 1.0, 0.0)));
			BOOST_CHECK( ! ayla::intersection::rayBox( ray, box, d ) );
		}

		Ray rayA( glm::vec3( 0.0, 6.0, 0.0 ), glm::vec3( 1.0, 0.0, 0.0)  );
		BOOST_CHECK( ! ayla::intersection::rayBox( rayA, box, d ) );

		Ray rayB(glm::vec3(0.0, 6.0, 0.0), glm::normalize(glm::vec3(1.0, -1.0, 0.0)));
		BOOST_CHECK( ayla::intersection::rayBox( rayB, box, d ) );

		Ray rayC(glm::vec3(10.0, 10.0, 10.0), glm::normalize(glm::vec3(-1.0, -1.0, -1.0)));
		BOOST_CHECK(  ayla::intersection::rayBox( rayC, box, d ) );
		
		transform.rotate( glm::pi<Float>() / 2.0f, glm::vec3( 0.0, 1.0, 0.0 ) );
		box.rotateBase( transform.getRotationMatrix() );
	}

	transform.rotate(glm::pi<Float>() / 4.0f, glm::vec3(0.0, 1.0, 0.0));
	box.rotateBase( transform.getRotationMatrix() );
	
	Ray rayA( glm::vec3( 5.0, 5.0, 5.0 ), glm::vec3( 1.0, 0.0, 0.0 ) );
	BOOST_CHECK( ! ayla::intersection::rayBox( rayA, box, d ) );
}

BOOST_AUTO_TEST_CASE( ray_sphere_intersection ) { 
	Sphere sphere( glm::vec3( 0.0, 0.0, 0.0 ), 10.0 );
	Float d;
	
	for (int i = 0; i < 10; i++) {
		Ray ray(glm::vec3(20.0, i, 0.0), glm::vec3(-1.0, 0.0, 0.0));
		BOOST_CHECK(ayla::intersection::raySphere( ray, sphere, d));
	}
	
	for (int i = -10; i < 10; i++) {
		for (int j = -10; j < 10; j++) {
			//inside out
			Ray rayA(glm::vec3(0.0, 0.0, 0.0), glm::normalize(glm::vec3(i, j, 10.0)));
			Ray rayB(glm::vec3(0.0, 0.0, 0.0), glm::normalize(glm::vec3(i, 10.0, j)));
			Ray rayC(glm::vec3(0.0, 0.0, 0.0), glm::normalize(glm::vec3(10.0, i, j)));
			BOOST_CHECK( ayla::intersection::raySphere(rayA, sphere, d ) );
			BOOST_CHECK( ayla::intersection::raySphere(rayB, sphere, d ) );
			BOOST_CHECK( ayla::intersection::raySphere(rayC, sphere, d ) );
		
			//outside in
			Ray rayD(glm::vec3(i, j, 10.0), glm::normalize(-glm::vec3(i, j, 10.0)));
			Ray rayE(glm::vec3(i, 10.0, j), glm::normalize(-glm::vec3(i, 10.0, j)));
			Ray rayF(glm::vec3(10.0, i, j), glm::normalize(-glm::vec3(10.0, i, j)));
			BOOST_CHECK( ayla::intersection::raySphere(rayD, sphere, d ) );
			BOOST_CHECK( ayla::intersection::raySphere(rayE, sphere, d ) );
			BOOST_CHECK( ayla::intersection::raySphere(rayF, sphere, d ) );
		
			//so if we change the ray direction it shouldn't intersect
			Ray rayG(glm::vec3(i, j, 11.0), glm::normalize(glm::vec3(i, j, 10.0)));
			Ray rayH(glm::vec3(i, 11.0, j), glm::normalize(glm::vec3(i, 10.0, j)));
			Ray rayI(glm::vec3(11.0, i, j), glm::normalize(glm::vec3(10.0, i, j)));
			BOOST_CHECK( ! ayla::intersection::raySphere(rayG, sphere, d ) );
			BOOST_CHECK( ! ayla::intersection::raySphere(rayH, sphere, d ) );
			BOOST_CHECK( ! ayla::intersection::raySphere(rayI, sphere, d ) );
		}
	}
}

BOOST_AUTO_TEST_CASE( ray_plane_intersection ) {
	// XZ plane
	Plane plane( glm::vec3( 0.0, 1.0, 0.0 ), 0.0 );
	Float d;
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			Ray ray(glm::vec3(i, 10.0, j), glm::normalize(-glm::vec3(i, 10.0, j)));
			BOOST_CHECK(ayla::intersection::rayPlane(ray, plane, d));

			Ray another(glm::vec3(i, 10.0, j), glm::normalize(glm::vec3(i, 10.0, j)));
			BOOST_CHECK(!ayla::intersection::rayPlane(another, plane, d));
		}
	}
	
	//parallel to the plane
	for (int i = 1; i < 10; i++) {
		for (int j = 1; j < 10; j++) {
			Ray parallel(glm::vec3(0.0, 1.0, 0.0), glm::normalize(glm::vec3(i, 0.0, j)));
			BOOST_CHECK(!ayla::intersection::rayPlane(parallel, plane, d));
		}
	}
	
	//inside the plane
	for (int i = 1; i < 10; i++) {
		for (int j = 1; j < 10; j++) {
			Ray inside(glm::vec3(0.0, 0.0, 0.0), glm::normalize(glm::vec3(i, 0.0, j)));
			BOOST_CHECK(ayla::intersection::rayPlane(inside, plane, d));
		}
	}
	
	Plane rotatedPlane(glm::normalize(glm::vec3( 1.0, 1.0, 0.0 )), 0.0);
	
	glm::vec3 offset(1.0, 0.0, 1.0);
	for (int j = 0; j < 10; j++ ) {
		glm::vec3 position = glm::vec3( 10.0, 10.0, 0.0 ) + offset;
		Ray ray(  position, glm::normalize(-position) );
		BOOST_CHECK( ayla::intersection::rayPlane( ray, rotatedPlane, d ) );
		
		Ray another( position, glm::normalize(position) );
		BOOST_CHECK( ! ayla::intersection::rayPlane( another, rotatedPlane, d ) );
		
		offset = glm::rotate<Float>(offset, glm::pi<Float>() / 8.0f, glm::vec3(1.0, 1.0, 0.0));
	}
	
	//parallel to the plane
	Ray parallel( glm::vec3( 1.0, 1.0, 0.0 ), glm::normalize(glm::vec3( 1.0, 0.0, 1.0 )) );
	for (int i = 1; i < 8; i++) {
		BOOST_CHECK( ! ayla::intersection::rayPlane( parallel, rotatedPlane, d ) );
		glm::vec3 rotatedDirection = glm::rotate<Float>(parallel.getDirection(), glm::pi<Float>() / 4.0f, glm::vec3(1.0, 1.0, 0.0));
		parallel.setDirection( rotatedDirection );
	}
	
	//inside the plane
	Ray inside( glm::vec3( 0.0, 0.0, 0.0 ), glm::normalize(glm::vec3( 1.0, 0.0, 1.0 )) );
	for (int i = 1; i < 8; i++) {
		BOOST_CHECK( ayla::intersection::rayPlane( inside, rotatedPlane, d ) );
		glm::vec3 rotatedDirection = glm::rotate<Float>(inside.getDirection(), glm::pi<Float>() / 4.0f, glm::vec3(1.0, 1.0, 0.0));
		inside.setDirection( rotatedDirection );
	}
	
	//Not in the origin
}

BOOST_AUTO_TEST_CASE( sphere_aab_intersection ) {
	Sphere sphere( glm::vec3( 0.0, 0.0, 0.0 ), 10.0 );
	
	for (int i = -11; i <= 11; i++) {
		AxisAlignedBox aab( glm::vec3( i, 0.0, 0.0 ), 1.0, 1.0, 1.0 );
		BOOST_CHECK( ayla::intersection::sphereAab( sphere, aab ) );
	}
	
	{
		AxisAlignedBox aab( glm::vec3( 0.0, 0.0, 0.0 ), 1.0, 1.0, 1.0 );
		for (int i = 0; i < 10; i++) {
			Sphere sphere( glm::vec3( 1.0 + cos( glm::pi<Float>()/4.0f), (0.1 * i) + sin( glm::pi<Float>()/4.0f), 0.0 ), 1.0  );
			BOOST_CHECK( ayla::intersection::sphereAab( sphere, aab ) );
		}
	}
	
	{
		AxisAlignedBox aab( glm::vec3( 0.0, 0.0, 0.0 ), 1.0, 1.0, 1.0 );
		for (int i = -10; i < 10; i++) {
			Sphere sphereA( glm::vec3( i, 2.0, 0.0 ), 0.5 );
			Sphere sphereB( glm::vec3( 2.0, i, 0.0 ), 0.5 );
			Sphere sphereC( glm::vec3( 0.0, 2.0, i ), 0.5 );
			
			BOOST_CHECK( ! ayla::intersection::sphereAab( sphereA, aab ) );
			BOOST_CHECK( ! ayla::intersection::sphereAab( sphereB, aab ) );
			BOOST_CHECK( ! ayla::intersection::sphereAab( sphereC, aab ) );
		}
	}
	
	{
		AxisAlignedBox aab( glm::vec3( 0.0, 0.0, 0.0 ), 10.0, 10.0, 10.0 );
		Sphere contained( glm::vec3( 1.0, 0.0, 1.0 ), 1.0 );
		Sphere containing( glm::vec3( 0.0, 1.0, 2.0 ), 100.0 );
		
		BOOST_CHECK( ayla::intersection::sphereAab( contained, aab) );
		BOOST_CHECK( ayla::intersection::sphereAab( containing, aab ) );
	}
}

BOOST_AUTO_TEST_CASE( triangle_line_segment_intersection ) {
	Triangle triangle(glm::vec3( 3.0, 0.0, 0.0 ), glm::vec3( 0.0, 4.0, 0.0 ), glm::vec3( 0.0, 0.0, 0.0 ));
	
	{
		LineSegment axisX( glm::vec3( -5.0, 4.0, 0.0 ), glm::vec3( 5.0, 4.0, 0.0 ) );
		LineSegment axisY( glm::vec3( 3.0, -5.0, 0.0 ), glm::vec3( 3.0, 5.0, 0.0 ) );
		LineSegment axisZ( glm::vec3( 0.0, 0.0, -5.0 ), glm::vec3( 0.0, 0.0, 5.0 ) );
		LineSegment segment( glm::vec3( 3.0, 4.0, 0.0 ), glm::vec3( 0.0, 0.0, 0.0 ) );
		LineSegment contained( glm::vec3( 0.1, 0.1, 0.0 ), glm::vec3( 0.2, 0.2, 0.0 ) );
		LineSegment inEdge( glm::vec3( 0.0, 0.0, 0.0 ), glm::vec3( 4.0, 0.0, 0.0 ) );
		glm::vec3 intersectionPoint;
		
		BOOST_CHECK( ayla::intersection::triangleLineSegment(triangle, segment) );
		BOOST_CHECK( ayla::intersection::triangleLineSegment(triangle, contained) );
		BOOST_CHECK( ayla::intersection::triangleLineSegment(triangle, inEdge ) );
		
		BOOST_CHECK( ayla::intersection::triangleLineSegment( triangle, axisX, intersectionPoint ) );
		BOOST_CHECK( ayla::epsilonEqual( intersectionPoint, glm::vec3( 0.0, 4.0, 0.0 ) ) );
		
		BOOST_CHECK( ayla::intersection::triangleLineSegment( triangle, axisY, intersectionPoint ) );
		BOOST_CHECK( ayla::epsilonEqual( intersectionPoint, glm::vec3( 3.0, 0.0, 0.0 ) ) );
		
		BOOST_CHECK( ayla::intersection::triangleLineSegment( triangle, axisZ, intersectionPoint ) );
		BOOST_CHECK( ayla::epsilonEqual( intersectionPoint, glm::vec3( 0.0, 0.0, 0.0 ) ) );
	}
}

BOOST_AUTO_TEST_CASE( plane_line_segment_intersection ) {
	Plane planeXZ( glm::vec3( 0.0, 1.0, 0.0 ), 0.0 );
	
	LineSegment contained( glm::vec3( 1.0, 0.0, 1.0 ) , glm::vec3( -1.0, 0.0, -2.0 ) );
	LineSegment parallel( glm::vec3( 1.0, 1.0, 1.0 ), glm::vec3( -1.0, 1.0, -2.0 ) );
	glm::vec3 intersectionPoint;
	
	BOOST_CHECK( ayla::intersection::planeLineSegment( planeXZ, contained, intersectionPoint ) );
	BOOST_CHECK( ayla::epsilonEqual( intersectionPoint, glm::vec3( 1.0, 0.0, 1.0 ) ) );
	
	BOOST_CHECK( ! ayla::intersection::planeLineSegment( planeXZ, parallel, intersectionPoint ) );
	
	for (int i = -10; i < 10; i++) {
		for (int j = -10; j < 10; j++) {
			LineSegment segment( glm::vec3( i, 10.0, j ), glm::vec3( 0.0, -1.0, 0.0 ) );
			BOOST_CHECK( ayla::intersection::planeLineSegment( planeXZ, segment, intersectionPoint ) );
		
			LineSegment pColliding( glm::vec3( 0.0, 0.0, 0.0 ) , glm::vec3( i, -1.0, j ) );
			BOOST_CHECK( ayla::intersection::planeLineSegment( planeXZ, pColliding, intersectionPoint ) );
			BOOST_CHECK( ayla::epsilonEqual( intersectionPoint, pColliding.getP() ) );
		
			LineSegment qColliding( glm::vec3( i, 1.0, j ), glm::vec3( 0.0, 0.0, 0.0 ) );
			BOOST_CHECK( ayla::intersection::planeLineSegment( planeXZ, qColliding, intersectionPoint ) );
			BOOST_CHECK( ayla::epsilonEqual( intersectionPoint, qColliding.getQ() ) );
		}
	}	
}

BOOST_AUTO_TEST_CASE( aab_line_segment_intersection ) {
	AxisAlignedBox aab( glm::vec3( 0.0, 0.0, 0.0 ), 10.0, 10.0, 10.0 );
	
	for (int i = -10; i < 10; i++) { 
		for (int j = -10; j < 10; j++) {
			LineSegment inwards( glm::vec3( i, 20.0, j ), glm::vec3( 0.0, 0.0, 0.0 ) );
			BOOST_CHECK( ayla::intersection::aabLineSegment( aab, inwards ) );
		
			LineSegment outwards( glm::vec3( i, 11.0, j ), glm::vec3( 2*i, 22.0, 2*j ) );
			BOOST_CHECK( ! ayla::intersection::aabLineSegment( aab, outwards ) );
		}
	}
	
	LineSegment parallel( glm::vec3( 20.0, 0.0, 0.0 ), glm::vec3( 20.0, 10.0, 0.0 ) );
	LineSegment contained( glm::vec3( -3.0, 0.0, 0.0 ), glm::vec3( 3.0, 0.0, 0.0 ) );
	LineSegment collidesVertex( glm::vec3( 20.0, 0.0, 10.0 ), glm::vec3( 0.0, 20.0, 10.0 ) );
	LineSegment collidesEdgeA( glm::vec3( 20.0, 0.0, 0.0 ), glm::vec3( 0.0, 20.0, 0.0 ) );
	LineSegment collidesEdgeB( glm::vec3( 20.0, 0.0, 0.0 ), glm::vec3( 0.0, 0.0, 20.0 ) );
	LineSegment collidesEdgeC( glm::vec3( 0.0, 20.0, 0.0 ), glm::vec3( 0.0, 0.0, 20.0 ) );
	
	BOOST_CHECK( ! ayla::intersection::aabLineSegment( aab, parallel ) );
	BOOST_CHECK( ayla::intersection::aabLineSegment( aab, contained ) );
	BOOST_CHECK( ayla::intersection::aabLineSegment( aab, collidesVertex ) );
	BOOST_CHECK( ayla::intersection::aabLineSegment( aab, collidesEdgeA ) );
	BOOST_CHECK( ayla::intersection::aabLineSegment( aab, collidesEdgeB ) );
	BOOST_CHECK( ayla::intersection::aabLineSegment( aab, collidesEdgeC ) );	
}

BOOST_AUTO_TEST_CASE( aab_plane_intersection ) {
	AxisAlignedBox aab( glm::vec3( 0.0, 0.0, 0.0 ), 10.0, 10.0, 10.0 );
	
	glm::vec3 normal( 0.0, 1.0, 0.0 );
	for (int i = 0; i < 10; i++) {
		Plane plane( normal, 0.0 );
		BOOST_CHECK( ayla::intersection::aabPlane( aab, plane ) );
		
		normal = glm::rotate<Float>( normal, glm::pi<Float>()/6.0f, glm::vec3( 1.0f, 0.0f, 0.0 ) );
	}
	
	Plane parallelToFace( glm::vec3( 0.0, 1.0, 0.0 ), 11.0 );
	Plane containsFace( glm::vec3( 0.0, 1.0, 0.0 ), 10.0 );
	Plane containsEdge( glm::normalize(glm::vec3( 1.0f, 1.0f, 0.0f )), sqrt(2.0f) * 10.0f );
	Plane farAway( glm::normalize(glm::vec3( 2.0, 3.3, -5.0 )), 100.0 );
	
	BOOST_CHECK( ! ayla::intersection::aabPlane( aab, parallelToFace ) );
	BOOST_CHECK( ayla::intersection::aabPlane( aab, containsFace ) );
	BOOST_CHECK( ayla::intersection::aabPlane( aab, containsEdge ) );
	BOOST_CHECK( ! ayla::intersection::aabPlane( aab, farAway ) );
}

BOOST_AUTO_TEST_CASE( aab_triangle_intersection ) { 
	AxisAlignedBox aab( glm::vec3( 0.0f, 0.0f, 0.0f ), 10.0f, 10.0f, 10.0f );
	
	glm::vec3 direction( 1.0f, 0.0f, 0.0f );
	
	for (int i = 0; i < 10; i++) {
		glm::vec3 origin( 0.0f, 11.0f, 0.0f );
		Triangle tri( origin, origin + direction * 10.0f, glm::vec3( 0.0f, 0.0f, 0.0f ) );
		BOOST_CHECK( ayla::intersection::aabTriangle( aab, tri ) );
		direction = glm::rotate<Float>( direction, glm::pi<Float>() / 6.0f, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	}
	
	Triangle contained( glm::vec3( -3.0f, 0.0f, 0.0f ), glm::vec3( 2.0f, 5.0f, -3.0f ), glm::vec3( 3.0f, 2.0f, 3.0f ) );
	Triangle perpendicularToFaceOut( glm::vec3( 12.0f, 0.0f, 0.0f ), glm::vec3( 20.0f, 0.0f, 0.0f ), glm::vec3( 13.0f, 5.0f, 0.0f ) );
	Triangle perpendicularToFaceIn( glm::vec3( 7.0f, 0.0f, 0.0f ), glm::vec3( 15.0f, 0.0f, 0.0f ), glm::vec3( 9.0f, 5.0f, 0.0f ) );
	Triangle parallelToFace( glm::vec3( 12.0f, 0.0f, 10.0f ), glm::vec3( 12.0f, 0.0f, -10.0f ), glm::vec3( 12.0f, 5.0f, 0.0f ) );
	Triangle containedFace( glm::vec3( 10.0f, 0.0f, -5.0f ), glm::vec3( 10.0f, 0.0f, 5.0f ), glm::vec3( 10.0f, 5.0f, 0.0f ) );
	Triangle containsFace( glm::vec3( 10.0f, -10.0f, -50.0f ), glm::vec3( 10.0f, -10.0f, 50.0f ), glm::vec3( 10.0f, 50.0f, 0.0f ) );
	Triangle collidingVertex( glm::vec3( 10.0f, 10.0f, 0.0f ), glm::vec3( 20.0f, 10.0f, 0.0f ), glm::vec3( 20.0f, 20.0f, 0.0f ) );
	Triangle collidingEdge( glm::vec3( 10.0f, -10.0f, -50.0f ), glm::vec3( 10.0f, -10.0f, 50.0f ), glm::vec3( 0.0f, -50.0f, 0.0f ) );
	
	BOOST_CHECK( ayla::intersection::aabTriangle( aab, contained ) );
	BOOST_CHECK( ! ayla::intersection::aabTriangle( aab, perpendicularToFaceOut ) );
	BOOST_CHECK( ayla::intersection::aabTriangle( aab, perpendicularToFaceIn ) );
	BOOST_CHECK( ! ayla::intersection::aabTriangle( aab, parallelToFace ) );
	BOOST_CHECK( ayla::intersection::aabTriangle( aab, containedFace ) );
	BOOST_CHECK( ayla::intersection::aabTriangle( aab, containsFace ) );
	BOOST_CHECK( ayla::intersection::aabTriangle( aab, collidingVertex ) );
	BOOST_CHECK( ayla::intersection::aabTriangle( aab, collidingEdge ) );
	
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


