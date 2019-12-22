#include <ayla/geometry/regular_grid_indexer.hpp>
#include <ayla/geometry/vector.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( ayla )
BOOST_AUTO_TEST_SUITE( regular_grid_indexer ) 

BOOST_AUTO_TEST_CASE( regular_grid_indexer_2d ) {
	const auto aabb = AxisAlignedBox(glm::vec3(2.5f, 2.5f, 0.0f), 2.5f, 2.5f, 0.0f);
	const auto aabbSize = aabb.getExtensions();
	const auto cellsSize = glm::vec3(0.5f, 0.5f, 1.0f);

	const auto grid = RegularGridIndexer(
		aabb, SizeType(10), SizeType(10), SizeType(1)
	);
	
	{ // indexes
		BOOST_CHECK(grid.contains(Index3D(4, 4, 0)));
		BOOST_CHECK(!grid.contains(Index3D(23, 15, 4)));
		
		auto index = Index3D(1, 2, 0);
		// index = 2*10 + 1
		BOOST_CHECK(grid.getOneDimensionalIndex(index) == 21);
	}
	
	{ // dimensions
		BOOST_CHECK(grid.getWidth() == 10);
		BOOST_CHECK(grid.getHeight() == 10);
		BOOST_CHECK(grid.getNumberOfCells() == 100);
	}
	
	{ // cells
		glm::vec3 center = grid.getCellCenter(Index3D(0, 0, 0));
		BOOST_CHECK( ayla::epsilonEqual( center, glm::vec3( 0.25f, 0.25f, 0.0f ) ) );
		
		center = grid.getCellCenter(Index3D(9, 9, 0));
		BOOST_CHECK( ayla::epsilonEqual( center, glm::vec3( 4.75f, 4.75f, 0.0f ) ) );
		
		Index3D index;
		index = grid.getIndexOfPoint(glm::vec3(3.0f, 4.0f, 0.0f));
		BOOST_CHECK( index[0] == 6 );
		BOOST_CHECK( index[1] == 8 );
		
		index = grid.getIndexOfPoint(glm::vec3(4.5f, 3.2f, 0.0f));
		BOOST_CHECK( index[0] == 9 );
		BOOST_CHECK( index[1] == 6 );
	}
	
	{ // line segment 
		LineSegment parallelX( glm::vec3( 1.0f, 2.0f, 0.0f ), glm::vec3( 3.0f, 2.0f, 0.0f )  );
		
		auto cells = grid.getCellsOfLineSegment(parallelX);
		
		BOOST_CHECK( cells.size() == 5 );
		BOOST_CHECK( cells[0] == Index3D( 2, 4, 0 ) );
		BOOST_CHECK( cells[1] == Index3D( 3, 4, 0 ) );
		BOOST_CHECK( cells[2] == Index3D( 4, 4, 0 ) );
		BOOST_CHECK( cells[3] == Index3D( 5, 4, 0 ) );
		BOOST_CHECK( cells[4] == Index3D( 6, 4, 0 ) );
		
		LineSegment diagonal( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 1.0f, 2.0f, 0.0f ) );
		cells = grid.getCellsOfLineSegment(diagonal);
		
		BOOST_CHECK( cells.size() == 7 );
		BOOST_CHECK( cells[0] == Index3D( 0, 0, 0 ) );
		BOOST_CHECK( cells[1] == Index3D( 0, 1, 0 ) );
		BOOST_CHECK( cells[2] == Index3D( 1, 1, 0 ) );
		BOOST_CHECK( cells[3] == Index3D( 1, 2, 0 ) );
		BOOST_CHECK( cells[4] == Index3D( 1, 3, 0 ) );
		BOOST_CHECK( cells[5] == Index3D( 2, 3, 0 ) );
		BOOST_CHECK( cells[6] == Index3D( 2, 4, 0 ) );		
	}
}

BOOST_AUTO_TEST_CASE( regular_grid_indexer_3d ) {
	const auto aabb = AxisAlignedBox(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, 10.0f, 10.0f);
	const auto aabbSize = aabb.getExtensions();
	const auto cellsSize = glm::vec3(1.0f, 1.0f, 1.0f);

	const auto grid = RegularGridIndexer(
		aabb, SizeType(20), SizeType(20), SizeType(20)
	);
	
	{ // indexes
		BOOST_CHECK(grid.contains(Index3D(4, 4, 4)));
		BOOST_CHECK(!grid.contains(Index3D(23, 15, 4)));
		
		auto index = Index3D(1, 2, 3);
		// index = 3 * (20*20) + 2*20 + 1
		BOOST_CHECK(grid.getOneDimensionalIndex(index) == 1241);
	}
	
	{ // dimensions
		BOOST_CHECK(ayla::epsilonEqual(grid.getOrigin(), glm::vec3(-10.0f, -10.0f, -10.0f)));
		BOOST_CHECK(grid.getWidth() == 20);
		BOOST_CHECK(grid.getHeight() == 20);
		BOOST_CHECK(grid.getDepth() == 20);
		BOOST_CHECK(grid.getNumberOfCells() == 20 * 20 * 20);
	}
	
	{ // cells
		auto center = grid.getCellCenter(Index3D(0, 0, 0));
		BOOST_CHECK( ayla::epsilonEqual( center, glm::vec3( -9.5f, -9.5f, -9.5f ) ) );
		
		center = grid.getCellCenter(Index3D(10, 10, 10));
		BOOST_CHECK( ayla::epsilonEqual( center, glm::vec3( 0.5f, 0.5f, 0.5f ) ) );
		
		Index3D index;
		index = grid.getIndexOfPoint(glm::vec3(3.0f, 4.0f, 5.0f));
		BOOST_CHECK( index[0] == 13 );
		BOOST_CHECK( index[1] == 14 );
		BOOST_CHECK( index[2] == 15 );
		
		index = grid.getIndexOfPoint(glm::vec3(-3.0f, 3.0f, 0.0f));
		BOOST_CHECK( index[0] == 7 );
		BOOST_CHECK( index[1] == 13 );
		BOOST_CHECK( index[2] == 10 );
	}
	
	{ // line segment 
		LineSegment parallelX( glm::vec3( 1.0f, 2.0f, 0.0f ), glm::vec3( 3.0f, 2.0f, 0.0f )  );
		
		auto cells = grid.getCellsOfLineSegment(parallelX);
		
		BOOST_CHECK( cells.size() == 3 );
		BOOST_CHECK(cells[0] == Index3D(11, 12, 10));
		BOOST_CHECK(cells[1] == Index3D(12, 12, 10));
		BOOST_CHECK(cells[2] == Index3D(13, 12, 10));
		
		LineSegment diagonal( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 1.0f, 2.0f, 0.0f ) );
		cells = grid.getCellsOfLineSegment(diagonal);
		
		BOOST_CHECK( cells.size() == 4 );
		BOOST_CHECK(cells[0] == Index3D(10, 10, 10));
		BOOST_CHECK(cells[1] == Index3D(10, 11, 10));
		BOOST_CHECK(cells[2] == Index3D(11, 11, 10));
		BOOST_CHECK(cells[3] == Index3D(11, 12, 10));
		
	}
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()