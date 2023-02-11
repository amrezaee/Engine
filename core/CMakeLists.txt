set(SOURCE_LIST "src/Application.cpp"
				"src/Assert.cpp"
				"src/Camera.cpp"
				"src/Color.cpp"
				"src/GPUBuffers.cpp"
				"src/GPUBuffersGL.cpp"
				"src/Logger.cpp"
				"src/RenderDevice.cpp"
				"src/RenderDeviceGL.cpp"
				"src/Renderer.cpp"
				"src/Shader.cpp"
				"src/ShaderGL.cpp"
				"src/Texture.cpp"
				"src/TextureGL.cpp"
				"src/Transform.cpp"
				"src/Window.cpp"
				"src/WindowGLFW.cpp"

				"../deps/stb_image.c"
				"../deps/glad/gl.c"
				
				"include/Application.hpp"
				"include/Assert.hpp"
				"include/Camera.hpp"
				"include/Color.hpp"
				"include/Common.hpp"
				"include/Delegate.hpp"
				"include/EntryPoint.hpp"
				"include/GPUBuffers.hpp"
				"include/GPUBuffersGL.hpp"
				"include/Image.hpp"
				"include/Logger.hpp"
				"include/MathFunctions.hpp"
				"include/RenderDevice.hpp"
				"include/RenderDeviceGL.hpp"
				"include/Renderer.hpp"
				"include/Shader.hpp"
				"include/ShaderGL.hpp"
				"include/Signal.hpp"
				"include/Texture.hpp"
				"include/TextureGL.hpp"
				"include/Timer.hpp"
				"include/Transform.hpp"
				"include/Vector2.hpp"
				"include/Window.hpp"
				"include/WindowGLFW.hpp"
)

include(FetchContent)

set(GLFW_INSTALL		OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS	OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY     https://github.com/glfw/glfw.git
	GIT_TAG            7482de6071d21db77a7236155da44c172a7f6c9e # 3.3.8
	FIND_PACKAGE_ARGS  3.3.0 
)

set(SPDLOG_INSTALL                OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE		  OFF CACHE BOOL "" FORCE)
set(SPDLOG_NO_EXCEPTIONS		  ON  CACHE BOOL "" FORCE)
set(SPDLOG_ENABLE_PCH			  ON  CACHE BOOL "" FORCE)
set(SPDLOG_DISABLE_DEFAULT_LOGGER ON  CACHE BOOL "" FORCE)
FetchContent_Declare(
	spdlog
	GIT_REPOSITORY    https://github.com/gabime/spdlog.git
	GIT_TAG           ad0e89cbfb4d0c1ce4d097e134eb7be67baebb36 # 1.11.0
	FIND_PACKAGE_ARGS 1.10.0
)

FetchContent_MakeAvailable(glfw spdlog)

add_library(core STATIC ${SOURCE_LIST})

target_include_directories(core PUBLIC "include/" "../deps/")

target_link_libraries(core PUBLIC glfw spdlog::spdlog)

target_compile_definitions(core PUBLIC
									$<$<CONFIG:Debug>:DEBUG_BUILD>
									
								PRIVATE
									GLFW_INCLUDE_NONE
)

if(GCC_CLI)
	target_compile_options(core PRIVATE
									-Wall -Weffc++ -Wctor-dtor-privacy -Woverloaded-virtual
									-fno-gnu-keywords
									-fno-exceptions -fno-rtti
)

elseif(MSVC_CLI)
	target_compile_options(core PRIVATE
									/GR-
									/options:strict /permissive- /W4 /nologo /Zc:__cplusplus /MP
)

	target_compile_definitions(core PRIVATE 
										_HAS_EXCEPTIONS=0
)

	string(REGEX REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

endif()
