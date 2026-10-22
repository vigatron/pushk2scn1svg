cmake_minimum_required(VERSION 3.16)
project(vhlibsvg VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

#
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/version_vhlibsvg.h"
)

#
add_library(vhlib_svg STATIC
    src/vhrect.hpp

    src/vhsvg.hpp
    src/vhsvg.cpp
    src/strtools.hpp
    src/strtools.cpp
)

# target_sources(vhlib_svg STATIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/vhlibbitptr.hpp
# )

#
target_include_directories(vhlib_svg PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

#
target_link_libraries(vhlib_svg PUBLIC vhlib_platform)
