#include <Shader.hpp>

#include <Assert.hpp>
#include <RenderDevice.hpp>
#include <ShaderGL.hpp>

ShaderPtr Shader::Create(const Path& path)
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeSptr<ShaderGL>(path);
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}
