cmake_minimum_required(VERSION 3.15)
project(smc40cr LANGUAGES C)
add_library("l${PROJECT_NAME}" STATIC startup.c logger.c)
target_compile_definitions("l${PROJECT_NAME}" PRIVATE MOCK_REGISTERS=1)

add_executable(${PROJECT_NAME} test_main.c)
target_link_libraries(${PROJECT_NAME} "l${PROJECT_NAME}")