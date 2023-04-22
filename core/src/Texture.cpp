#include <Texture.hpp>

#include <Assert.hpp>
#include <RenderDevice.hpp>
#include <TextureGL.hpp>

TexturePtr Texture::Create()
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeShared<TextureGL>();
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}

TexturePtr Texture::Create(const Path& path)
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeShared<TextureGL>(path);
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}

TexturePtr Texture::Create(
        u32 width, u32 height, bool filter, WrapMode wrap, Color border)
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL:
		return MakeShared<TextureGL>(width, height, filter, wrap, border);
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}

bool Texture::operator==(const Texture& rhs) const
{
	return GetID() == rhs.GetID();
}

bool Texture::operator!=(const Texture& rhs) const
{
	return !operator==(rhs);
}
