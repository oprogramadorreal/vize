# glew-cmake project
#
# Provides a cmake build system for the GLEW project, and offers binary releases
# across several different platforms and compilers
#

# We require a minimum of V2.8 of CMake
cmake_minimum_required( VERSION 2.8 )

# Project name (Will determine the output name)
project( glew )

# Setup the version numbers for the release we're currently at
set( LIB_VMAJOR 1 )
set( LIB_VMINOR 10 )
set( LIB_VPATCH  0 )

# Determine the pointer size to know what architecture we're targetting
if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
    set( PSIZE 32 )
elseif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( PSIZE 64 )
else()
    message( FATAL_ERROR "Data width could not be determined!" )
endif()

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set( DBG "-debug" )
endif()

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
    set( COMPILER "-gcc" )
elseif( MSVC )
    set( COMPILER "-vc" )

    # With MSVC we should normally generate a more specific compiler version
    # which includes information about the revision of the compiler. This
    # revision information lets us know which of the MSVCR libraries is depended
    # upon in our build.
    #
    # However, GLEW doesn't use the CRT so there is no need to differentiate
    # between different CRT versions.

endif()

if( WIN32 )
    list( APPEND MLIBS opengl32 )
    list( APPEND MLIBS gdi32 )
    list( APPEND MLIBS user32 )
    list( APPEND MLIBS kernel32 )
endif()

if( UNIX )
    list( APPEND MLIBS GL )
    list( APPEND MLIBS X11 )
endif()

if( COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
    set( CMAKE_C_FLAGS_RELEASE "-Wall -O2" )
    set( CMAKE_C_FLAGS_DEBUG "-Wall -O0 -g" )
elseif( MSVC )
    set( CMAKE_C_FLAGS_RELEASE "/O2" )
    set( CMAKE_C_FLAGS_DEBUG "/Zi /Od" )
endif()

# Build the library name from the project and databus width (pointer size)
set( GLEW_LIB "${PROJECT_NAME}${PSIZE}" CACHE STRING "Glew library name" )

# Include directories for all targets
include_directories( "${PROJECT_SOURCE_DIR}/include" )

# C Pre-processor definitions for all targets
add_definitions( -DGLEW_BUILD )
add_definitions( -DGLEW_NO_GLU )

# The shared GLEW library
add_library( ${GLEW_LIB} SHARED
    "${PROJECT_SOURCE_DIR}/src/glew.c"
    "${PROJECT_SOURCE_DIR}/build/glew.rc" )
	
target_link_libraries( ${GLEW_LIB}
    ${MLIBS} )

# The static GLEW library
add_library( ${GLEW_LIB}s STATIC
    "${PROJECT_SOURCE_DIR}/src/glew.c"
    "${PROJECT_SOURCE_DIR}/build/glew.rc" )

set_target_properties( ${GLEW_LIB}s PROPERTIES
    COMPILE_DEFINITIONS "GLEW_STATIC" )

# The mx shared GLEW library
add_library( ${GLEW_LIB}mx SHARED
    "${PROJECT_SOURCE_DIR}/src/glew.c"
    "${PROJECT_SOURCE_DIR}/build/glew.rc" )

set_target_properties( ${GLEW_LIB}mx PROPERTIES
    COMPILE_DEFINITIONS "GLEW_MX" )

target_link_libraries( ${GLEW_LIB}mx
    ${MLIBS} )

# The mx static GLEW library
add_library( ${GLEW_LIB}mxs STATIC
    "${PROJECT_SOURCE_DIR}/src/glew.c"
    "${PROJECT_SOURCE_DIR}/build/glew.rc" )

set_target_properties( ${GLEW_LIB}mxs PROPERTIES
    COMPILE_DEFINTIIONS "GLEW_STATIC GLEW_MX" )

install( TARGETS ${GLEW_LIB} ${GLEW_LIB}s
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib )

install( TARGETS ${GLEW_LIB}mx ${GLEW_LIB}mxs
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    COMPONENT mx )

# The glewinfo application
add_executable( glewinfo
    "${PROJECT_SOURCE_DIR}/src/glewinfo.c" )

# Build the glewinfo application statically linked
target_link_libraries( glewinfo
    ${GLEW_LIB}s
    ${MLIBS} )

install( TARGETS glewinfo
    RUNTIME DESTINATION bin )

# The visualinfo application
add_executable( visualinfo
    "${PROJECT_SOURCE_DIR}/src/visualinfo.c" )

target_link_libraries( visualinfo
    ${GLEW_LIB}s
    ${MLIBS} )

install( TARGETS visualinfo
    RUNTIME DESTINATION bin )

# Header files need to be included in the install!
install( DIRECTORY ${PROJECT_SOURCE_DIR}/include/GL
    DESTINATION include )

# --- CPack Packaging
# For packaging, select the package types we want to create
list( APPEND CPACK_GENERATOR TBZ2 )
list( APPEND CPACK_GENERATOR ZIP )

set( CPACK_PACKAGE_VERSION_MAJOR ${LIB_VMAJOR} )
set( CPACK_PACKAGE_VERSION_MINOR ${LIB_VMINOR} )
set( CPACK_PACKAGE_VERSION_PATCH ${LIB_VPATCH} )

set( CPACK_PACKAGE_NAME "${PROJECT_NAME}${COMPILER}${DBG}" )

# Do not move this include, it must be included after ALL CPack variables have
# been defined
include( CPack )
