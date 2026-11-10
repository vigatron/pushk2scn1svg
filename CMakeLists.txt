cmake_minimum_required(VERSION 3.16)

project(pushk2scn1svg VERSION 0.1.0 LANGUAGES CXX)

# set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_BUILD_TYPE "MinSizeRel")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)


# Подключаем папку с субмодулем
add_subdirectory(src/extmods/vhlibplatform)

# Подключаем папку с субмодулем
add_subdirectory(src/extmods/vhlibargs)

# Подключаем папку с субмодулем
add_subdirectory(src/extmods/vhlibfsys)

# Подключаем папку с субмодулем
add_subdirectory(src/extmods/vhlibsvg)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mods/vhappcorecfg/app_version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/app_version.hpp
    @ONLY
)

#
add_executable(pushk2scn1svg
    src/main.cpp

    src/mods/vhlogger/vhlogger.cpp
    src/mods/global/global.cpp
 
    src/mods/widerange/vhkeyfingerprint.hpp
    src/mods/widerange/vhkeyfingerprint.cpp

    src/mods/widerange/widerange.hpp
    src/mods/widerange/widerange.cpp

    src/mods/generatewnd/generatewnd.cpp
    src/mods/generatedna/generatedna.cpp

    src/mods/vhappcore/vhappcore.cpp
    src/mods/vhappcorecfg/vhappcorecfg.cpp
    src/mods/datasource/vhsrcdataarray.cpp

    src/mods/colors/colors.cpp
    src/mods/svgtools/svgtools.hpp
    src/mods/svgtools/svgtools.cpp
    src/mods/vhsvgdraw/vhsvgdraw.cpp

    src/mods/vhsha256/VHSHA256.cpp
)

#
target_include_directories(${PROJECT_NAME} PUBLIC
    src

    src/mods

    src/mods/vhlogger
    src/mods/global

    src/mods/widerange
    src/mods/vhsha256

    src/extmods
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

# Defines
# target_compile_definitions(${PROJECT_NAME} PRIVATE
#     # CFG_DISABLE_CRC_CACHE=1
#     VHPLATFORM_PC
# )

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    vhlib_platform
    vhlib_args
    vhlib_fsys
    vhlib_svg
)
