include(FetchContent)

set(GLFW_INSTALL		OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS	OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY		https://github.com/glfw/glfw.git
	GIT_TAG				7482de6071d21db77a7236155da44c172a7f6c9e # 3.3.8
	FIND_PACKAGE_ARGS	3.3.0 
)

set(SPDLOG_INSTALL                OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE		  OFF CACHE BOOL "" FORCE)
set(SPDLOG_NO_EXCEPTIONS		  ON  CACHE BOOL "" FORCE)
set(SPDLOG_ENABLE_PCH			  ON  CACHE BOOL "" FORCE)
set(SPDLOG_DISABLE_DEFAULT_LOGGER ON  CACHE BOOL "" FORCE)
FetchContent_Declare(
	spdlog
	GIT_REPOSITORY		https://github.com/gabime/spdlog.git
	GIT_TAG				ad0e89cbfb4d0c1ce4d097e134eb7be67baebb36 # 1.11.0
	FIND_PACKAGE_ARGS	1.10.0
)

FetchContent_Declare(
	entt
	GIT_REPOSITORY		https://github.com/skypjack/entt.git
	GIT_TAG				fef921132cae7588213d0f9bcd2fb9c8ffd8b7fc # 3.11.1
	FIND_PACKAGE_ARGS   3.10.0
)

FetchContent_MakeAvailable(glfw spdlog entt)

add_library(deps STATIC "stb_image.c" "glad/gl.c")
target_include_directories(deps PUBLIC "/")

add_library(core-deps INTERFACE)
target_link_libraries(core-deps INTERFACE deps glfw spdlog::spdlog EnTT::EnTT)
target_compile_definitions(core-deps INTERFACE ENTT_NOEXCEPTION ENTT_USE_ATOMIC ENTT_STANDARD_CPP)