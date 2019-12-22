cmake_minimum_required(VERSION 3.1)

project(lucca_qt)

set(LUCCA_QT_INCLUDE_DIR
	${LUCCA_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
set(LUCCA_QT_LIBRARY lucca_qt ${LUCCA_LIBRARY})

include_directories(${Boost_INCLUDE_DIR})
include_directories(${LUCCA_QT_INCLUDE_DIR})

file(GLOB_RECURSE LUCCA_QT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE LUCCA_QT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_library(lucca_qt SHARED ${LUCCA_QT_HEADERS} ${LUCCA_QT_SOURCES})
target_link_libraries(${LUCCA_QT_LIBRARY} ${Boost_LIBRARIES})
qt5_use_modules(lucca_qt Widgets OpenGL)

set_target_properties(lucca_qt PROPERTIES COMPILE_FLAGS "-DLUCCA_QT_LIB")

set(LUCCA_QT_INCLUDE_DIR ${LUCCA_QT_INCLUDE_DIR} PARENT_SCOPE)
set(LUCCA_QT_LIBRARY ${LUCCA_QT_LIBRARY} PARENT_SCOPE)

install(TARGETS lucca_qt
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

source_group_tree("Header Files" include)
source_group_tree("Source Files" src)
