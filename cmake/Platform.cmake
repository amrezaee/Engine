# Platform Detection

# Endianness
if(CMAKE_CXX_BYTE_ORDER STREQUAL "BIG_ENDIAN")
    set(ENGINE_PLATFORM_BIG_ENDIAN TRUE)

elseif(CMAKE_CXX_BYTE_ORDER STREQUAL "LITTLE_ENDIAN")
    set(ENGINE_PLATFORM_LITTLE_ENDIAN TRUE)

else()
    MESSAGE(FATAL_ERROR "Unknown endianness")
endif()

# OS
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(ENGINE_PLATFORM_WINDOWS TRUE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(ENGINE_PLATFORM_ANDROID TRUE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(ENGINE_PLATFORM_LINUX TRUE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(ENGINE_PLATFORM_EMSCRIPTEN TRUE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(ENGINE_PLATFORM_MACOSX TRUE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(ENGINE_PLATFORM_IOS TRUE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "tvOS")
    set(ENGINE_PLATFORM_TVOS TRUE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "watchOS")
    set(ENGINE_PLATFORM_WATCHOS TRUE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "MSYS")
    set(ENGINE_PLATFORM_MSYS TRUE)

elseif(CMAKE_SYSTEM_NAME STREQUAL "CYGWIN")
    set(ENGINE_PLATFORM_CYGWIN TRUE)

else()
    message(FATAL_ERROR "Unknown platform")
endif()

if(UNIX)
    set(ENGINE_PLATFORM_UNIX TRUE)
endif()

if(APPLE)
    set(ENGINE_PLATFORM_APPLE TRUE)
endif()

# Processor
if(ENGINE_PLATFORM_WINDOWS)

    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        set(ENGINE_CPU_X86_64 TRUE)

    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64")
        set(ENGINE_CPU_ARM64 TRUE)

    else()
        set(ENGINE_CPU_UNKNOWN TRUE)
    endif()

elseif(ENGINE_PLATFORM_MACOSX)

    if(CMAKE_APPLE_SILICON_PROCESSOR STREQUAL "x86_64")
        set(ENGINE_CPU_X86_64 TRUE)

    elseif(CMAKE_APPLE_SILICON_PROCESSOR STREQUAL "arm64")
        set(ENGINE_CPU_ARM64 TRUE)

    else()
        set(ENGINE_CPU_UNKNOWN TRUE)
    endif()

elseif(ENGINE_PLATFORM_UNIX)

    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(ENGINE_CPU_X86_64 TRUE)

    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(ENGINE_CPU_ARM64 TRUE)

    else()
        set(ENGINE_CPU_UNKNOWN TRUE)
    endif()

else()
    set(ENGINE_CPU_UNKNOWN TRUE)
endif()

function(define_platform_info target)
    target_compile_definitions(${target} PRIVATE
                               ENGINE_PLATFORM_BIG_ENDIAN=$<BOOL:${ENGINE_PLATFORM_BIG_ENDIAN}>
                               ENGINE_PLATFORM_LITTLE_ENDIAN=$<BOOL:${ENGINE_PLATFORM_LITTLE_ENDIAN}>

                               ENGINE_PLATFORM_WINDOWS=$<BOOL:${ENGINE_PLATFORM_WINDOWS}>
                               ENGINE_PLATFORM_ANDROID=$<BOOL:${ENGINE_PLATFORM_ANDROID}>
                               ENGINE_PLATFORM_LINUX=$<BOOL:${ENGINE_PLATFORM_LINUX}>
                               ENGINE_PLATFORM_EMSCRIPTEN=$<BOOL:${ENGINE_PLATFORM_EMSCRIPTEN}>
                               ENGINE_PLATFORM_MACOSX=$<BOOL:${ENGINE_PLATFORM_MACOSX}>
                               ENGINE_PLATFORM_IOS=$<BOOL:${ENGINE_PLATFORM_IOS}>
                               ENGINE_PLATFORM_TVOS=$<BOOL:${ENGINE_PLATFORM_TVOS}>
                               ENGINE_PLATFORM_WATCHOS=$<BOOL:${ENGINE_PLATFORM_WATCHOS}>
                               ENGINE_PLATFORM_MSYS=$<BOOL:${ENGINE_PLATFORM_MSYS}>
                               ENGINE_PLATFORM_CYGWIN=$<BOOL:${ENGINE_PLATFORM_CYGWIN}>
                               ENGINE_PLATFORM_UNIX=$<BOOL:${ENGINE_PLATFORM_UNIX}>
                               ENGINE_PLATFORM_APPLE=$<BOOL:${ENGINE_PLATFORM_APPLE}>

                               ENGINE_CPU_X86_64=$<BOOL:${ENGINE_CPU_X86_64}>
                               ENGINE_CPU_ARM64=$<BOOL:${ENGINE_CPU_ARM64}>
                               ENGINE_CPU_UNKNOWN=$<BOOL:${ENGINE_CPU_UNKNOWN}>
                               )
endfunction()

message(STATUS "-PLATFORM         = ${CMAKE_SYSTEM_NAME}")

if(ENGINE_CPU_X86_64)
    message(STATUS "-CPU              = X86_64")
elseif(ENGINE_CPU_ARM64)
    message(STATUS "-CPU              = ARM64")
else()
    message(STATUS "-CPU              = Unknown")
endif()

if(ENGINE_PLATFORM_LITTLE_ENDIAN)
    message(STATUS "-ENDIANNESS       = Little")
else()
    message(STATUS "-ENDIANNESS       = Big")
endif()
