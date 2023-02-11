#include <Texture.hpp>

#include <Assert.hpp>
#include <RenderDevice.hpp>
#include <TextureGL.hpp>

TexturePtr Texture::Create()
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeSptr<TextureGL>();
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}

TexturePtr Texture::Create(const Path& path)
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeSptr<TextureGL>(path);
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}

TexturePtr Texture::Create(U32 width, U32 height, bool filter, WrapMode wrap,
                           Color border)
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL:
		return MakeSptr<TextureGL>(width, height, filter, wrap, border);
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