set(CORE_HEADERS "include/Application.hpp"
				 "include/Assert.hpp"
				 "include/Camera.hpp"
				 "include/Color.hpp"
				 "include/Common.hpp"
				 "include/Components.hpp"
				 "include/Delegate.hpp"
				 "include/Entity.hpp"
				 "include/EntryPoint.hpp"
				 "include/GPUBuffers.hpp"
				 "include/GPUBuffersGL.hpp"
				 "include/Logger.hpp"
				 "include/MathFunctions.hpp"
				 "include/RenderDevice.hpp"
				 "include/RenderDeviceGL.hpp"
				 "include/Renderer.hpp"
				 "include/Scene.hpp"
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

set(CORE_SOURCES "src/Application.cpp"
				 "src/Assert.cpp"
				 "src/Camera.cpp"
				 "src/Color.cpp"
				 "src/Entity.cpp"
				 "src/GPUBuffers.cpp"
				 "src/GPUBuffersGL.cpp"
				 "src/Logger.cpp"
				 "src/RenderDevice.cpp"
				 "src/RenderDeviceGL.cpp"
				 "src/Renderer.cpp"
				 "src/Scene.cpp"
				 "src/Shader.cpp"
				 "src/ShaderGL.cpp"
				 "src/Texture.cpp"
				 "src/TextureGL.cpp"
				 "src/Transform.cpp"
				 "src/Window.cpp"
				 "src/WindowGLFW.cpp"
)

file(GLOB SHADER_SOURCES "../shaders/*.glsl")

add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS} ${SHADER_SOURCES})

DEFINE_COMPILER_INFO(core)
DEFINE_PLATFORM_INFO(core)

target_include_directories(core PUBLIC "include/")

target_link_libraries(core PUBLIC core-deps)

target_compile_definitions(core PUBLIC
									$<$<CONFIG:Debug>:DEBUG_BUILD>
									
								PRIVATE
									GLFW_INCLUDE_NONE
)


if(ENGINE_COMPILER_GCC_VARIANT)
	
	target_compile_options(core PRIVATE
									-Wall -Weffc++ -Wctor-dtor-privacy -Woverloaded-virtual
									-fno-gnu-keywords
									-fno-exceptions #-fno-rtti
	)

elseif(ENGINE_COMPILER_MSVC_VARIANT)

	target_compile_options(core PRIVATE
									#/GR-
									/options:strict /permissive- /W4 /nologo /Zc:__cplusplus /MP /fp:fast
	)

	target_compile_definitions(core PRIVATE 
										_HAS_EXCEPTIONS=0
	)

endif()

source_group("include" FILES ${CORE_HEADERS})
source_group("src"     FILES ${CORE_SOURCES})
source_group("shader"  FILES ${SHADER_SOURCES})
