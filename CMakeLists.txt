cmake_minimum_required(VERSION 3.16)

project(vhpackv81scan VERSION 0.1.0 LANGUAGES CXX)

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


#
add_executable(vhpackv81scan
    src/main.cpp

    src/mods/dbg/dbg.cpp
    src/mods/global/global.cpp
 
    src/mods/widerange/vhkeyfingerprint.hpp
    src/mods/widerange/vhkeyfingerprint.cpp

    src/mods/widerange/widerange.hpp
    src/mods/widerange/widerange.cpp

    src/mods/generatewnd/generatewnd.cpp
    src/mods/generatedna/generatedna.cpp

    src/mods/cmdline/cmdline.cpp

    src/mods/draw/colors.cpp
    src/mods/draw/draw.cpp
    src/mods/draw/draw_density.cpp
    src/mods/draw/draw_scale.cpp
    src/mods/draw/draw_section.cpp
    src/mods/draw/draw_types.cpp
)

#
target_include_directories(${PROJECT_NAME} PUBLIC
    src

    src/mods

    src/mods/dbg
    src/mods/global

    src/mods/widerange
    
    src/extmods
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

    # vhlib_bitptr
    # vhlib_metacrc
    # vhlib_timerstamp
    # vhlib_log
)

# add_custom_command(TARGET ${PRJNAME} POST_BUILD COMMAND
#     cp "${PROJECT_BINARY_DIR}/${PRJNAME}"     "$ENV{HOME}/bin/${PRJNAME}"        )
