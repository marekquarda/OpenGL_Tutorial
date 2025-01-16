cmake_minimum_required(VERSION 3.20)        

project(OpenGL_Other1)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#option(SDL_SHARED "" OFF)
#option(SDL_STATIC "" ON)
add_subdirectory(glfw)
add_subdirectory(glm)

# Load all files in src folder
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB RES_FILES "res/*")

add_executable(${PROJECT_NAME} ${SRC_FILES}) 

file(COPY ${RES_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME}  glfw glm)


