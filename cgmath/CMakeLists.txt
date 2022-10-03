set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE CGMATH_SRC CONFIGURE_DEPENDS src/*.cc include/*.h)
add_library(cgmath ${CGMATH_SRC})
target_include_directories(cgmath PUBLIC include PRIVATE src)