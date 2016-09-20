cmake_minimum_required(VERSION 2.8)

project(depends)
add_subdirectory("glew/build/cmake" "glew")
add_subdirectory("glfw" "glfw")
add_subdirectory("SOIL" "SOIL")
add_subdirectory("glm" "glm")
