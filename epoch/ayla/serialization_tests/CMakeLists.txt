cmake_minimum_required(VERSION 2.8)

project(ayla_serialization_tests)

include_directories(
  ${Boost_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

file(GLOB_RECURSE ayla_serialization_tests_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(ayla_serialization_tests ${ayla_serialization_tests_SOURCES})
target_link_libraries(ayla_serialization_tests ayla ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

TARGET_DYNAMIC_LIBRARIES(ayla_serialization_tests
  ayla
)
TARGET_BOOST_DYNAMIC_LIBRARIES(ayla_serialization_tests)

add_test(NAME ayla_serialization_tests
         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIR}
         COMMAND ayla_serialization_tests
)