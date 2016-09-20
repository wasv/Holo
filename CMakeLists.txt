cmake_minimum_required(VERSION 2.8)
project( Holo CXX )
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")
set(DEP_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/dependencies")

set(GLEW_LIBRARY "libglew32d")
set(SOIL_LIBRARY "SOIL")
set(GLFW_LIBRARY "glfw")

find_package( OpenGL REQUIRED )

add_subdirectory("${DEP_SOURCE_DIR}")

#add_subdirectory("${DEP_ROOT_DIR}/glew/build/cmake" "${CMAKE_BINARY_DIR}/lib/glew")
#add_subdirectory("${DEP_ROOT_DIR}/glfw" "${CMAKE_BINARY_DIR}/lib/glfw")
#add_subdirectory("${DEP_ROOT_DIR}/SOIL" "${CMAKE_BINARY_DIR}/lib/SOIL")

add_definitions(-DGLEW_STATIC)

link_directories("${CMAKE_CURRENT_BINARY_DIR}/lib")
link_directories("${DEP_BINARY_DIR}/glfw/src")
link_directories("${DEP_BINARY_DIR}/SOIL")

include_directories("${DEP_SOURCE_DIR}/glew/include")
include_directories("${DEP_SOURCE_DIR}/glfw/include")
include_directories("${DEP_SOURCE_DIR}/SOIL/src")
include_directories("${DEP_SOURCE_DIR}/glm")

configure_file("h2g2coin.obj" "objs/h2g2coin.obj" COPYONLY)
configure_file("bb8.obj" "objs/bb8.obj" COPYONLY)
configure_file("CSH.obj" "objs/CSH.obj" COPYONLY)

add_executable(holo main.cpp)
target_link_libraries(holo ${GLFW_LIBRARY} ${OPENGL_LIBRARY} ${GLEW_LIBRARY} ${SOIL_LIBRARY})
