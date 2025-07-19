cmake_minimum_required(VERSION 3.19)

project(izgRayTracer)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

option(SDL_SHARED "" OFF)
option(SDL_STATIC "" ON)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)
file(GLOB SRC_FILES "src/*.*")


add_executable(${PROJECT_NAME} ${SRC_FILES})

add_custom_target(run ./${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
        glm
)
