cmake_minimum_required(VERSION 3.15)
project(smc40cr LANGUAGES C CXX)

add_subdirectory(googletest)


#####################################################################################################
set(project ${PROJECT_NAME}_test)
add_executable(${project} test_main.c SMC_40CR.c startup.c utils/logger.c utils/timer_mock.c )
target_compile_definitions(${project} PRIVATE MOCK_REGISTERS=1)
target_include_directories(${project} PRIVATE . utils)

#####################################################################################################
#Tests
enable_testing()
include(GoogleTest)

#####################################################################################################
#Fake Function Framework tests
set(project ${PROJECT_NAME}_fff)
add_executable(${project} tests/CALL_ORDER_TEST.cpp startup.c utils/logger.c utils/timer_mock.c )
target_link_libraries(${project} gtest gtest_main)
target_compile_definitions(${project} PRIVATE FFF_GENERATE=ON FFF_UNIT_TESTING=ON MOCK_REGISTERS=1)
target_include_directories(${project} PRIVATE . utils tests)
gtest_discover_tests(${project})

#####################################################################################################
#Google Tests
set(project ${PROJECT_NAME}_gtests)
add_executable(${project} tests/UNIT_TESTS.cpp SMC_40CR.c startup.c utils/logger.c utils/timer_mock.c)
target_link_libraries(${project} gtest gtest_main)
target_compile_definitions(${project} PRIVATE MOCK_REGISTERS=1)
target_include_directories(${project} PRIVATE . utils tests)
gtest_discover_tests(${project})

#####################################################################################################
add_custom_target(run_tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS ${project}
)

