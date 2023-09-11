cmake_minimum_required(VERSION 3.18)
project(RtApi VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Python Interpreter n shit
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# add_subdirectory(folder)
# set(FLIB folder)

add_subdirectory(cgmath)
set(CGMATH cgmath) 

# file(GLOB SOURCES "src/*.cc")

# add_executable(rtapi ${SOURCES})
add_executable(oscpub "src/osc_pub.cc")
target_link_libraries(oscpub zmq pthread m ${Boost_LIBRARIES})

add_executable(rtapi "src/main.cc")

set (CMAKE_SHARED_LINKER_FLAGS "-DIMGUI_IMPL_OPENGL_LOADER_GLEW,`pkg-config --static --libs glfw3`,`pkg-config --cflags glfw3`")
target_link_libraries(rtapi zmq pthread cgmath GLEW glfw GL m X11 ${PYTHON_LIBRARIES})
