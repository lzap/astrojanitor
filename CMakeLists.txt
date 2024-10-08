cmake_minimum_required(VERSION 3.15)
project(test)

find_package(raylib 4.5 REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} src/cubicmap.c src/main.c)

target_link_libraries(${PROJECT_NAME} raylib)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
