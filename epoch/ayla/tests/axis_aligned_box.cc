#include <boost/test/unit_test.hpp>
#include <ayla/geometry/axis_aligned_box.hpp>
#include <ayla/geometry/vector.hpp>

#include <iostream>
#include <stdio.h>
#include <fstream>

BOOST_AUTO_TEST_SUITE( ayla )
BOOST_AUTO_TEST_SUITE( axis_aligned_box )

BOOST_AUTO_TEST_CASE( intersectsTest ) {
	const AxisAlignedBox a(glm::vec3(5,5,5), 2,2,2);
	AxisAlignedBox b(glm::vec3(5,4,5), 1,1,1);
	
	BOOST_CHECK(a.intersects(b));
	b.translate(glm::vec3(0, -2, 0));
	BOOST_CHECK(a.intersects(b));
	b.translate(glm::vec3(0,-1,0));
	BOOST_CHECK(!a.intersects(b));
	b.translate(glm::vec3(1,1,0));
	BOOST_CHECK(a.intersects(b));
}

BOOST_AUTO_TEST_CASE( volumeTest ) {
	const AxisAlignedBox a(glm::vec3(134,431,2), 1,2,3);
	
	BOOST_CHECK_EQUAL(a.getVolume(), 2*4*6);
}

BOOST_AUTO_TEST_CASE( extremePointsTest ) {
	const glm::vec3 aabCenter(23,43,123);
	const glm::vec3 hExts(10,15,20);
	const glm::vec3 maxPt = aabCenter + hExts;
	const glm::vec3 minPt = aabCenter - hExts;
	
	const AxisAlignedBox a(aabCenter, hExts.x, hExts.y, hExts.z);

	BOOST_CHECK(a.getMaxPoint() == maxPt);
	BOOST_CHECK(a.getMinPoint() == minPt);
	
	glm::vec3 posPt, negPt;
	a.getDirExtremePts(glm::normalize(glm::vec3(1,1,0)), posPt, negPt);
		
	BOOST_CHECK(negPt == minPt);
	BOOST_CHECK(posPt == maxPt);
	
	a.getDirExtremePts(glm::normalize(glm::vec3(1,-1,-1)), posPt, negPt);
	
	BOOST_CHECK(negPt == (aabCenter+glm::vec3(-hExts.x, hExts.y,hExts.z)));
	BOOST_CHECK(posPt == (aabCenter+glm::vec3(hExts.x,-hExts.y,-hExts.z)));
}

BOOST_AUTO_TEST_CASE( dimensionsTest ) {
	const glm::vec3 minPt(20,30,40);
	const glm::vec3 maxPt(25,35,45);

	const AxisAlignedBox a(minPt, maxPt);
	
	BOOST_CHECK(a.getCenter() == (minPt+maxPt)*0.5f);
	
	BOOST_CHECK_EQUAL(a.getWidth(), maxPt.x-minPt.x);
	BOOST_CHECK_EQUAL(a.getHeight(), maxPt.y-minPt.y);
	BOOST_CHECK_EQUAL(a.getDepth(), maxPt.z-minPt.z);
	
	BOOST_CHECK_EQUAL(a.getHalfWidth(), 0.5f*(maxPt.x-minPt.x));
	BOOST_CHECK_EQUAL(a.getHalfHeight(), 0.5f*(maxPt.y-minPt.y));
	BOOST_CHECK_EQUAL(a.getHalfDepth(), 0.5f*(maxPt.z-minPt.z));
}

BOOST_AUTO_TEST_CASE( constructionTest ) {
	const glm::vec3 center(-20,120,-100);
	const glm::vec3 hExts(2,4,5);

	const glm::vec3 minPt = center-hExts;
	const glm::vec3 maxPt = center+hExts;
	
	AxisAlignedBox a(center, hExts.x, hExts.y, hExts.z);
	const AxisAlignedBox b(minPt, maxPt);
	
	BOOST_CHECK(a.getCenter() == b.getCenter());
	BOOST_CHECK(a.getHalfExtensions() == b.getHalfExtensions());
	BOOST_CHECK(a.getVolume() == b.getVolume());
	
	BOOST_CHECK(a.getCenter() == center);
	a.setCenter(glm::vec3(0));
	
	BOOST_CHECK(a.getCenter() == glm::vec3(0));
	BOOST_CHECK(a.getHalfExtensions() == b.getHalfExtensions());
	
	a.setCenter(center);
	
	BOOST_CHECK(a.getCenter() == b.getCenter());
	BOOST_CHECK(a.getHalfExtensions() == b.getHalfExtensions());
}

BOOST_AUTO_TEST_CASE( clip_line_segment ) {
	AxisAlignedBox aab( glm::vec3( 0.0f, 0.0f, 0.0f ), 10.0f, 10.0f, 10.0f );
	LineSegment clipped;
	
	LineSegment outside( glm::vec3( 20.0f, 0.0f, 0.0f ), glm::vec3( 22.0f, 30.0f, 5.0f )  );
	LineSegment parallelOutside( glm::vec3( 20.0f, 0.0f, 0.0f ), glm::vec3( 20.0f, 10.0f, 0.0f ) );
	
	BOOST_CHECK( ! aab.clipLineSegment( outside, clipped ) );
	BOOST_CHECK( ! aab.clipLineSegment( parallelOutside, clipped ) );
	
	LineSegment contained( glm::vec3( -5.0f, 0.0f, 0.0f ), glm::vec3( 5.0f, 0.0f, 0.0f ) );
	
	BOOST_CHECK( aab.clipLineSegment( contained, clipped ) );
	BOOST_CHECK( ayla::epsilonEqual( clipped.getP(), glm::vec3( -5.0f, 0.0f, 0.0f ) ) );
	BOOST_CHECK( ayla::epsilonEqual( clipped.getQ(), glm::vec3( 5.0f, 0.0f, 0.0f ) ) );

	LineSegment onFace( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 15.0f, 0.0f, 0.0f ) );
	
	BOOST_CHECK( aab.clipLineSegment( onFace, clipped ) );
	BOOST_CHECK( ayla::epsilonEqual( clipped.getP(), glm::vec3( 0.0f, 0.0f, 0.0f ) ) );
	BOOST_CHECK( ayla::epsilonEqual( clipped.getQ(), glm::vec3( 10.0f, 0.0f, 0.0f ) ) );
	
	LineSegment onEdge( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 14.0f, -14.0f, 0.0f ) );
	
	BOOST_CHECK( aab.clipLineSegment( onEdge, clipped ) );
	BOOST_CHECK( ayla::epsilonEqual( clipped.getP(), glm::vec3( 0.0f, 0.0f, 0.0f ) ) );
	BOOST_CHECK( ayla::epsilonEqual( clipped.getQ(), glm::vec3( 10.0f, -10.0f, 0.0f ) ) );
	
	LineSegment onVertex( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( -16.0f, 16.0f, -16.0f ) );
	
	BOOST_CHECK( aab.clipLineSegment( onVertex, clipped ) );
	BOOST_CHECK( ayla::epsilonEqual( clipped.getP(), glm::vec3( 0.0f, 0.0f, 0.0f ) ) );
	BOOST_CHECK( ayla::epsilonEqual( clipped.getQ(), glm::vec3( -10.0f, 10.0f, -10.0f ) ) );
	
	LineSegment diagonal( glm::vec3( -50.0f, -50.0f, -50.0f ), glm::vec3( 50.0f, 50.0f, 50.0f ) );
	
	BOOST_CHECK( aab.clipLineSegment( diagonal, clipped ) );
	BOOST_CHECK( ayla::epsilonEqual( clipped.getP(), glm::vec3( -10.0f, -10.0f, -10.0f ), 0.000001f ) );
	BOOST_CHECK(ayla::epsilonEqual( clipped.getQ(), glm::vec3(10.0f, 10.0f, 10.0f), 0.000001f ) );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()