cmake_minimum_required(VERSION 3.20)        

project(OpenGLimg)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#option(SDL_SHARED "" OFF)
#option(SDL_STATIC "" ON)
add_subdirectory(glfw)

# Load all files in src folder
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB RES_FILES "res/*")

add_executable(${PROJECT_NAME} ${SRC_FILES}) 

target_link_libraries(${PROJECT_NAME}  glfw)


