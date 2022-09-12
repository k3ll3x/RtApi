cmake_minimum_required(VERSION 3.19)
project(RtApi VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# add_subdirectory(folder)
# set(FLIB folder)

file(GLOB SOURCES "src/*.cc")

add_executable(rtapi ${SOURCES})
# target_link_libraries(rtapis)