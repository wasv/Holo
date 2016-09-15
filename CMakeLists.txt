cmake_minimum_required(VERSION 2.8)
project( Holo )

find_package( OpenGL REQUIRED )

set(DEP_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")

add_subdirectory("${DEP_ROOT_DIR}/glew/build/cmake" "${CMAKE_BINARY_DIR}/lib/glew")
add_subdirectory("${DEP_ROOT_DIR}/glfw" "${CMAKE_BINARY_DIR}/lib/glfw")
add_subdirectory("${DEP_ROOT_DIR}/SOIL" "${CMAKE_BINARY_DIR}/lib/SOIL")

add_definitions(-DGLEW_STATIC)

link_directories("${CMAKE_BINARY_DIR}/lib/Debug")
link_directories("${CMAKE_BINARY_DIR}/lib/glfw/src/Debug")
link_directories("${CMAKE_BINARY_DIR}/lib/SOIL/Debug")

include_directories("${DEP_ROOT_DIR}/glew/include")
include_directories("${DEP_ROOT_DIR}/glfw/include")
include_directories("${DEP_ROOT_DIR}/SOIL/src")
include_directories("${DEP_ROOT_DIR}/glm")

configure_file("h2g2coin.obj" "Debug/h2g2coin.obj" COPYONLY)
configure_file("bb8.obj" "Debug/bb8.obj" COPYONLY)
configure_file("CSH.obj" "Debug/CSH.obj" COPYONLY)

add_executable(holo main.cpp)
target_link_libraries(holo glfw ${OPENGL_LIBRARY} libglew32d SOIL)
