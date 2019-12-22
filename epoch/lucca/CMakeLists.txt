cmake_minimum_required(VERSION 3.1)

project(lucca)

set(LUCCA_INCLUDE_DIR
	${AYLA_GL_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
set(LUCCA_LIBRARY lucca ${AYLA_GL_LIBRARY})

include_directories(${Boost_INCLUDE_DIR})
include_directories(${LUCCA_INCLUDE_DIR})

file(GLOB_RECURSE LUCCA_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE LUCCA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_library(lucca SHARED ${LUCCA_HEADERS} ${LUCCA_SOURCES})
target_link_libraries(${LUCCA_LIBRARY} ${Boost_LIBRARIES})

set_target_properties(lucca PROPERTIES COMPILE_FLAGS "-DLUCCA_LIB")

set(LUCCA_INCLUDE_DIR ${LUCCA_INCLUDE_DIR} PARENT_SCOPE)
set(LUCCA_LIBRARY ${LUCCA_LIBRARY} PARENT_SCOPE)

install(TARGETS lucca
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

source_group_tree("Header Files" include)
source_group_tree("Source Files" src)
