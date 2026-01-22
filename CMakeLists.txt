cmake_minimum_required(VERSION 3.15)
project(smc40cr LANGUAGES C CXX)


add_subdirectory(googletest)

add_library("l${PROJECT_NAME}" STATIC startup.c logger.c)
target_compile_definitions("l${PROJECT_NAME}" PRIVATE MOCK_REGISTERS=1 TEST_STATIC_METHODS=1)

add_executable(${PROJECT_NAME} test_main.c)
target_link_libraries(${PROJECT_NAME} "l${PROJECT_NAME}")
target_include_directories(${PROJECT_NAME} PRIVATE)

###
enable_testing()
add_executable("${PROJECT_NAME}_test" test_gtests.cpp)
target_link_libraries("${PROJECT_NAME}_test" "l${PROJECT_NAME}" gtest gtest_main)
target_compile_definitions("${PROJECT_NAME}_test" PRIVATE MOCK_REGISTERS=1 TEST_STATIC_METHODS=1)
include(GoogleTest)
gtest_discover_tests("${PROJECT_NAME}_test")
add_custom_target(run_tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS "${PROJECT_NAME}_test"
)
