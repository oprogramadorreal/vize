cmake_minimum_required(VERSION 3.1)

project(vize)

if(WIN32)
	if (NOT DEFINED ENV{QTDIR})
		message(FATAL_ERROR "QTDIR environment variable is not defined!")
	endif()

	if (NOT DEFINED ENV{BOOST_ROOT})
		message(FATAL_ERROR "BOOST_ROOT environment variable is not defined!")
	endif()
endif()

set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../3rd-party)
if (NOT EXISTS ${3RD_PARTY_DIR})
	message(FATAL_ERROR "3RD_PARTY_DIR cannot be found!")
endif()

set(EPOCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../epoch)
if (NOT EXISTS ${EPOCH_DIR})
	message(FATAL_ERROR "EPOCH_DIR cannot be found!")
endif()

enable_testing()

option(AYLA_BUILD_TESTS "Build AYLA tests." ON)
option(AYLA_GL_BUILD_SAMPLES "Build AYLA-GL samples." ON)

set(CMAKE_DEBUG_POSTFIX _d)

if (MSVC)
	# wd4251 wd4275: disable "needs to have dll-interface" warning
	# wd4996: disable unsafe warning for non _s functions
	add_definitions("/wd4251 /wd4275 /wd4996")
endif()

if (AYLA_GL_BUILD_SAMPLES)
	include(${3RD_PARTY_DIR}/add_glfw.cmake)
	ADD_GLFW(${3RD_PARTY_DIR})
endif()

add_subdirectory(${3RD_PARTY_DIR}/glew 3rd-party/glew)

include(${3RD_PARTY_DIR}/add_gdcm.cmake)
ADD_GDCM(${3RD_PARTY_DIR})

set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(Qt5Widgets 5.4.1 REQUIRED)
find_package(Qt5OpenGL 5.4.1 REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${EPOCH_DIR}/CMakeModules")
include(source_group_tree)
#include(thirdparty)
include(target_libraries)

if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__forceinline=inline -std=c++14 -Wall")
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

add_subdirectory(${EPOCH_DIR}/ayla epoch/ayla)
add_subdirectory(${EPOCH_DIR}/ayla_gl epoch/ayla_gl)
add_subdirectory(${EPOCH_DIR}/lucca epoch/lucca)
add_subdirectory(${EPOCH_DIR}/lucca_qt epoch/lucca_qt)

cmake_policy(SET CMP0020 NEW) # http://qt-project.org/forums/viewthread/30006
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x64")
set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed.

FILE (GLOB_RECURSE VIZE_UIS ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui)
QT5_WRAP_UI(VIZE_UI_HEADERS ${VIZE_UIS})

set(QRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc )
QT5_ADD_RESOURCES(QRC_OUTFILES ${QRC_FILES})

if (WIN32)
	set(RC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.rc")
endif()

source_group("Resource Files" FILES ${QRC_FILES} ${RC_FILES})

include_directories(
	${LUCCA_QT_INCLUDE_DIR}
	${AYLA_GL_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${Boost_INCLUDE_DIR}
	${GDCM_INCLUDE_DIR}
)

FILE(GLOB_RECURSE VIZE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
FILE(GLOB_RECURSE VIZE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
FILE(GLOB_RECURSE VIZE_SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/*.fs ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/*.vs)

add_executable(${PROJECT_NAME} ${VIZE_INCLUDES} ${VIZE_SOURCES} ${VIZE_SHADERS} ${VIZE_UI_HEADERS} ${RC_FILES} ${QRC_OUTFILES})

target_link_libraries(${PROJECT_NAME} gdcmMSFF ${LUCCA_QT_LIBRARY} ${AYLA_GL_LIBRARY})
qt5_use_modules(${PROJECT_NAME} Widgets OpenGL)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

TARGET_DYNAMIC_LIBRARIES(${PROJECT_NAME}
	${LUCCA_QT_LIBRARY}
	gdcmMSFF
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::OpenGL
)
TARGET_BOOST_DYNAMIC_LIBRARIES(${PROJECT_NAME})
TARGET_QT_DYNAMIC_LIBRARIES(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

if (WIN32)
	file(GLOB _boost_dlls "${Boost_LIBRARY_DIRS}/*.dll")
	install(FILES ${_boost_dlls} DESTINATION "bin/") # TODO: Improve this. We are copying all boost dlls!

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

	install(
		FILES "$<TARGET_FILE:Qt5::QICOPlugin>"
			"$<TARGET_FILE:Qt5::QJpegPlugin>"
			"$<TARGET_FILE:Qt5::QTiffPlugin>"
		DESTINATION "bin/imageformats/"
	)

	if (AYLA_GL_BUILD_SAMPLES)
		install(FILES "$<TARGET_FILE:glfw>" DESTINATION "bin/")
	endif()
endif()

set_target_properties (ayla PROPERTIES FOLDER epoch/ayla)
set_target_properties (ayla_gl PROPERTIES FOLDER epoch/ayla)
set_target_properties (ayla_gl_sample_fps_camera PROPERTIES FOLDER epoch/ayla/samples)
set_target_properties (ayla_gl_sample_hello_world PROPERTIES FOLDER epoch/ayla/samples)
set_target_properties (ayla_gl_sample_text_renderer PROPERTIES FOLDER epoch/ayla/samples)
set_target_properties (ayla_gl_sample_trackball_camera PROPERTIES FOLDER epoch/ayla/samples)
set_target_properties (ayla_tests PROPERTIES FOLDER epoch/ayla/tests)
set_target_properties (ayla_serialization_tests PROPERTIES FOLDER epoch/ayla/tests)
set_target_properties (lucca PROPERTIES FOLDER epoch/lucca)
set_target_properties (lucca_qt PROPERTIES FOLDER epoch/lucca)

set_target_properties (glfw PROPERTIES FOLDER 3rd-party/glfw)
set_target_properties (uninstall PROPERTIES FOLDER 3rd-party/glfw)
set_target_properties (gdcmcharls PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmCommon PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmDICT PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmDSED PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmexpat PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmgetopt PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmIOD PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmjpeg12 PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmjpeg16 PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmjpeg8 PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmMEXD PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmMSFF PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmopenjpeg PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (gdcmzlib PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (socketxx PROPERTIES FOLDER 3rd-party/gdcm)
set_target_properties (glew64 PROPERTIES FOLDER 3rd-party/glew)
set_target_properties (glew64mx PROPERTIES FOLDER 3rd-party/glew)
set_target_properties (glew64mxs PROPERTIES FOLDER 3rd-party/glew)
set_target_properties (glew64s PROPERTIES FOLDER 3rd-party/glew)
set_target_properties (glewinfo PROPERTIES FOLDER 3rd-party/glew)
set_target_properties (visualinfo PROPERTIES FOLDER 3rd-party/glew)

if (MSVC)
	target_compile_options(socketxx PRIVATE /wd4297 /wd4244 /wd4005)
	target_compile_options(gdcmopenjpeg PRIVATE /wd4018 /wd4244 /wd4311 /wd4267)
	target_compile_options(gdcmjpeg8 PRIVATE /wd4267)
	target_compile_options(gdcmjpeg12 PRIVATE /wd4267)
	target_compile_options(gdcmjpeg16 PRIVATE /wd4267)
	target_compile_options(gdcmMEXD PRIVATE /wd4005 /wd4101)
	target_compile_options(gdcmMSFF PRIVATE /wd4244 /wd4267 /wd4146 /wd26451 /wd26495 /wd26439 /wd6011 /wd6262)
	target_compile_options(gdcmCommon PRIVATE /wd4267)
	target_compile_options(gdcmexpat PRIVATE /wd4244)
endif()

source_group_tree("Header Files" include)
source_group_tree("Source Files" src)
source_group_tree("Shader Files" resources/shaders)
source_group_tree("UI Files" ui)
