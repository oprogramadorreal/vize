cmake_minimum_required(VERSION 3.1)

project(epoch)

if (NOT DEFINED ENV{QTDIR})
	message(WARNING "QTDIR environment variable is not defined!")
endif()

if (NOT DEFINED ENV{BOOST_ROOT})
	message(FATAL_ERROR "BOOST_ROOT environment variable is not defined!")
endif()

set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../3rd-party)
if (NOT EXISTS ${3RD_PARTY_DIR})
	message(FATAL_ERROR "3RD_PARTY_DIR cannot be found!")
endif()

enable_testing()

option(AYLA_BUILD_TESTS "Build AYLA tests." ON)
option(AYLA_GL_BUILD_SAMPLES "Build AYLA-GL samples." ON)

set(CMAKE_DEBUG_POSTFIX _d)

if (AYLA_GL_BUILD_SAMPLES)
	include(${3RD_PARTY_DIR}/add_glfw.cmake)
	ADD_GLFW(${3RD_PARTY_DIR})
endif()

add_subdirectory(${3RD_PARTY_DIR}/glm 3rd-party/glm)
add_subdirectory(${3RD_PARTY_DIR}/glew 3rd-party/glew)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
include(source_group_tree)
#include(thirdparty)

list(APPEND CMAKE_MODULE_PATH "${3RD_PARTY_DIR}/cmake-scripts")
include(target_libraries)

if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__forceinline=inline -std=c++14 -Wall -fvisibility=hidden")
endif()

if (MSVC)
	# wd4251: disable "needs to have dll-interface" warning
	# wd4996: disable unsafe warning for non _s functions
	add_definitions("/wd4251 /wd4996")
endif()

add_definitions(-DBOOST_ALL_NO_LIB) # disable boost autolink

find_package(Boost 1.58.0 COMPONENTS
	system
	filesystem
	thread
	serialization
	unit_test_framework
	REQUIRED
)

add_subdirectory(ayla)
add_subdirectory(ayla_gl)
add_subdirectory(lucca)

if (DEFINED ENV{QTDIR})
	set(CMAKE_PREFIX_PATH $ENV{QTDIR})
	find_package(Qt5Widgets 5.4.1 REQUIRED)
	find_package(Qt5OpenGL 5.4.1 REQUIRED)

	add_subdirectory(lucca_qt)

	if (WIN32)
		install(FILES "$<TARGET_FILE:Qt5::QWindowsIntegrationPlugin>" DESTINATION "bin/platforms")

		STRING(REGEX REPLACE "\\\\" "/" icu_regexp "$ENV{QTDIR}/bin/icu*.dll")
		file(GLOB icu_dlls ${icu_regexp})

		install(
		  FILES "$<TARGET_FILE:Qt5::Core>"
				"$<TARGET_FILE:Qt5::Gui>"
				"$<TARGET_FILE:Qt5::Widgets>"
				"$<TARGET_FILE:Qt5::OpenGL>"
				${icu_dlls}
			DESTINATION "bin/"
		)
	endif()
else()
	message(WARNING "QTDIR environment variable is not defined!")
endif()

if (AYLA_GL_BUILD_SAMPLES)
	install(FILES "$<TARGET_FILE:glfw>" DESTINATION "bin/")
endif()

if (WIN32)
	file(GLOB _boost_dlls "${Boost_LIBRARY_DIRS}/*.dll")
	install(FILES ${_boost_dlls} DESTINATION "bin/") # TODO: Improve this. We are copying all boost dlls!
endif()
