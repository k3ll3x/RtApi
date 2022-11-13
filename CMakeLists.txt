cmake_minimum_required(VERSION 3.18)
project(RtApi VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# add_subdirectory(folder)
# set(FLIB folder)

add_subdirectory(cgmath)
set(CGMATH cgmath) 

file(GLOB SOURCES "src/*.cc")

add_executable(rtapi ${SOURCES})
set (CMAKE_SHARED_LINKER_FLAGS "-DIMGUI_IMPL_OPENGL_LOADER_GLEW,`pkg-config --static --libs glfw3`,`pkg-config --cflags glfw3`")
target_link_libraries(rtapi zmq pthread cgmath GLEW glfw GL m X11)