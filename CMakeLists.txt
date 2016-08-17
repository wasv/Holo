cmake_minimum_required(VERSION 2.8)
project( Holo )

find_package( OpenGL REQUIRED )

set(DEP_ROOT_DIR "C:/Users/waste/Documents/Projects/Holo/dependencies")

link_directories("${DEP_ROOT_DIR}/lib")
add_definitions(-DGLEW_STATIC)
include_directories("${DEP_ROOT_DIR}/include")

configure_file("h2g2coin.obj" "Debug/h2g2coin.obj" COPYONLY)
configure_file("tex.png" "Debug/tex.png" COPYONLY)

add_executable(holo main.cpp)
target_link_libraries(holo glfw3 ${OPENGL_LIBRARY} glew32s libglew32 SOIL)
