cmake_minimum_required(VERSION 3.19)

project(izgRayTracer)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

option(SDL_SHARED "" OFF)
option(SDL_STATIC "" ON)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)


add_executable(${PROJECT_NAME} src/main.cpp)

add_custom_target(run ./${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
        glm
)
