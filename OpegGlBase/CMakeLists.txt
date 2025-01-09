cmake_minimum_required(VERSION 3.20)        

project(OpenGL_tutorial)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#option(SDL_SHARED "" OFF)
#option(SDL_STATIC "" ON)
add_subdirectory(glfw)
include_directories(res)

# Load all files in src folder
file(GLOB SRC_FILES "src/*.cpp")
set(RES_FILES "res/default.frag" "res/default.vert")
#file(GLOB RES_FILES "res/*")

file(COPY ${RES_FILES}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRC_FILES} ${RES_FILES}) 

#target_sources(${PROJECT_NAME} PUBLIC ${RES_FILES})
target_link_libraries(${PROJECT_NAME}  glfw)

