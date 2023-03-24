include(FetchContent)

set(GLFW_INSTALL OFF)
set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 7482de6071d21db77a7236155da44c172a7f6c9e # 3.3.8
        FIND_PACKAGE_ARGS 3.3.0
)

FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG fef921132cae7588213d0f9bcd2fb9c8ffd8b7fc # 3.11.1
        FIND_PACKAGE_ARGS 3.10.0
)

FetchContent_MakeAvailable(glfw entt)

add_library(deps STATIC "stb_image.c" "glad/gl.c")
target_include_directories(deps PUBLIC "/")

add_library(core-deps INTERFACE)
target_link_libraries(core-deps INTERFACE deps glfw EnTT::EnTT)
target_compile_definitions(core-deps INTERFACE ENTT_NOEXCEPTION ENTT_USE_ATOMIC ENTT_STANDARD_CPP)
