cmake_minimum_required(VERSION 3.20)        

project(OpenGL_tutorial)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#option(SDL_SHARED "" OFF)
#option(SDL_STATIC "" ON)
add_subdirectory(glfw)
#include_directories(res)

# Load all files in src folder
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB RES_FILES "res/*")

#set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)
#set(RES_FILES "res/default.frag" "res/default.vert")
#set(SHADERS "default.frag" "default.vert")
#file(GLOB SHADERS ${SHADER_DIR}/*.vert ${SHADER_DIR}/*.frag ${SHADER_DIR}/*.comp ${SHADER_DIR}/*.geom ${SHADER_DIR}/*.tesc ${SHADER_DIR}/*.tese ${SHADER_DIR}/*.mesh ${SHADER_DIR}/*.task ${SHADER_DIR}/*.rgen ${SHADER_DIR}/*.rchit ${SHADER_DIR}/*.rmiss)

add_executable(${PROJECT_NAME} ${SRC_FILES}) 
#target_sources(${PROJECT_NAME} PUBLIC ${SHADERS})
#target_include_directories(${PROJECT_NAME} ${SHADERS})

file(COPY ${RES_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res)

target_link_libraries(${PROJECT_NAME}  glfw)


