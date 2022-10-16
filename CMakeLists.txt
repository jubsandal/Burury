cmake_minimum_required(VERSION 3.21)
project(Burury)

set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
        set(DEFAULT_BUILD_TYPE "Debug")
        set(CMAKE_BUILD_TYPE "Debug")
endif()

# export compile_commands

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
IF(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
        EXECUTE_PROCESS(COMMAND
                ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
                ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
ENDIF()

include(PocoMacros.cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT
        NAMES Qt6
        COMPONENTS
                Core
                Widgets
                Network
                Sql
                Core5Compat
        REQUIRED
        )

find_package(
        Qt${QT_VERSION_MAJOR}
        COMPONENTS
                Core
                Widgets
                Network
                Sql
                Core5Compat
        REQUIRED
        )

set(GLOBAL_RESOURCES ${CMAKE_SOURCE_DIR}/resources.qrc)

set(BUILD_SHARED_LIBS ON)
set(BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

FILE(GLOB libMakeFiles RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/*/CMakeLists.txt
        )
foreach (f ${libMakeFiles})
        get_filename_component(dir ${f} DIRECTORY)
        add_subdirectory(${dir})
endforeach()

add_subdirectory(Burury)
add_subdirectory(BururyDaemon)
