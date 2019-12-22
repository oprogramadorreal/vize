cmake_minimum_required(VERSION 2.8.9)

project(ayla)

if (NOT 3RD_PARTY_DIR)
	message(SEND_ERROR "3RD_PARTY_DIR is not defined!")
endif()

set(AYLA_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${3RD_PARTY_DIR}/glm
)
set(AYLA_LIBRARY ayla)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${3RD_PARTY_DIR}/glm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE AYLA_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE AYLA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_library(ayla SHARED ${AYLA_HEADERS} ${AYLA_SOURCES})
target_link_libraries(ayla ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SERIALIZATION_LIBRARY})

set_target_properties(ayla PROPERTIES COMPILE_FLAGS "-DAYLA_LIB")

if (AYLA_BUILD_TESTS)
	add_subdirectory(tests)
	add_subdirectory(serialization_tests)
endif()

set(AYLA_INCLUDE_DIR ${AYLA_INCLUDE_DIR} PARENT_SCOPE)
set(AYLA_LIBRARY ${AYLA_LIBRARY} PARENT_SCOPE)

install(TARGETS ayla
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

source_group_tree("Source Files" src)
source_group_tree("Header Files" include)