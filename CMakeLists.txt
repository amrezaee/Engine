cmake_minimum_required(VERSION 3.20.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
include(cmake/IDE.cmake) # TODO: it's not working, fix it.

project(Engine VERSION 0.0.0)

include(cmake/Compiler.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH              "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

option(ENGINE_STATIC_CRT    "Enable/Disable MSVC static crt" TRUE)
option(ENGINE_BUILD_SANDBOX "Build sandbox project"          TRUE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

if(ENGINE_STATIC_CRT)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

add_subdirectory(core)

if(ENGINE_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()
