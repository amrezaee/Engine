# Compiler Detection
if (CMAKE_CXX_COMPILER_ID STREQUAL "ClangApple")
    set(CLANG_APPLE TRUE CACHE BOOL "")
    message(STATUS "Compiler: Apple Clang")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        set(CLANG_CL TRUE CACHE BOOL "")
        message(STATUS "Compiler: Microsoft Clang-cl")
    elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        set(CLANG TRUE CACHE BOOL "")
        message(STATUS "Compiler: Clang")
    endif()

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(GCC TRUE CACHE BOOL "")
    message(STATUS "Compiler: GCC")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(MSVC TRUE CACHE BOOL "")
    message(STATUS "Compiler: MSVC")

else()
    message(FATAL "Unknown Compiler.")

endif()

if(CLANG OR GCC)
	set(GCC_CLI TRUE CACHE BOOL "")
else()
	set(MSVC_CLI TRUE CACHE BOOL "")
endif()

set(COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
message(STATUS "Compiler Version: ${COMPILER_VERSION}")