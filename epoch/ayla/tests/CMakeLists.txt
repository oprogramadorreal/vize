cmake_minimum_required(VERSION 2.8)

project(ayla_tests)

include_directories(
  ${Boost_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

file(GLOB_RECURSE AYLA_TESTS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(ayla_tests ${AYLA_TESTS_SOURCES})
target_link_libraries(ayla_tests ayla ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

TARGET_DYNAMIC_LIBRARIES(ayla_tests
  ayla
)
TARGET_BOOST_DYNAMIC_LIBRARIES(ayla_tests)

add_test(NAME ayla_tests
         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIR}
         COMMAND ayla_tests
)