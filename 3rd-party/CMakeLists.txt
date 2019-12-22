cmake_minimum_required(VERSION 3.1)

project(3rd-party)

enable_testing()

set(CMAKE_DEBUG_POSTFIX _d)
set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${3RD_PARTY_DIR}/glew 3rd-party/glew)

add_subdirectory(${3RD_PARTY_DIR}/glm 3rd-party/glm)
include(${3RD_PARTY_DIR}/add_glfw.cmake)
ADD_GLFW(${3RD_PARTY_DIR})

include(${3RD_PARTY_DIR}/add_gdcm.cmake)
ADD_GDCM(${3RD_PARTY_DIR})
