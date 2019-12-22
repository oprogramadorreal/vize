cmake_minimum_required(VERSION 3.1)

project(ayla_gl)

if (NOT 3RD_PARTY_DIR)
	message(SEND_ERROR "3RD_PARTY_DIR is not defined!")
endif()

set(AYLA_GL_INCLUDE_DIR 
	${3RD_PARTY_DIR}/glew/include
	${AYLA_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
set(AYLA_GL_LIBRARY
	ayla_gl
	${AYLA_LIBRARY}
	${GLEW_LIB}
)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${AYLA_GL_INCLUDE_DIR})

file(GLOB_RECURSE AYLA_GL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE AYLA_GL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE AYLA_GL_SHADERS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*)

add_library(ayla_gl SHARED ${AYLA_GL_HEADERS} ${AYLA_GL_SOURCES} ${AYLA_GL_SHADERS_SOURCES})
target_link_libraries(${AYLA_GL_LIBRARY})

set_target_properties(ayla_gl PROPERTIES COMPILE_FLAGS "-DAYLA_GL_LIB")

if (AYLA_GL_BUILD_SAMPLES)
	add_subdirectory(samples)
endif()

set(AYLA_GL_INCLUDE_DIR ${AYLA_GL_INCLUDE_DIR} PARENT_SCOPE)
set(AYLA_GL_LIBRARY ${AYLA_GL_LIBRARY} PARENT_SCOPE)

install(TARGETS ayla_gl
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION bin/epoch/ayla_gl)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION bin/epoch/ayla_gl)

source_group_tree("Source Files" src)
source_group_tree("Header Files" include)
source_group_tree("Shader Files" shaders)