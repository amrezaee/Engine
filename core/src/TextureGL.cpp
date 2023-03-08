#include <TextureGL.hpp>

#include <Assert.hpp>

TextureGL::TextureGL()
        : mWidth(1), mHeight(1), mDataFormat(GL_RGBA), mInternalFormat(GL_RGBA8)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &mID);
	glTextureStorage2D(mID, 1, mInternalFormat, mWidth, mHeight);
	SetFilter(false);
	SetWrapMode(WrapMode::Repeat);
	byte white[4] {0xff, 0xff, 0xff, 0xff};
	SetData(white, 4);
}

TextureGL::TextureGL(const Path& path)
{
	std::ifstream     in(path, std::ios::binary | std::ios::ate);
	std::vector<byte> image;
	int               w, h, c;
	stbi_uc*          raw;

	ASSERT(in, "Could not open file");

	size_t size = in.tellg();
	ASSERT(size != -1, "Could not read from file");
	image.resize(size);
	in.seekg(0, std::ios::beg);
	in.read((char*)&image[0], size);

	raw = stbi_load_from_memory(&image[0], int(size), &w, &h, &c, 0);
	ASSERT(raw, "Invalid image file");

	mWidth  = w;
	mHeight = h;

	if(c == 4)
	{
		mInternalFormat = GL_RGBA8;
		mDataFormat     = GL_RGBA;
	}
	else if(c == 3)
	{
		mInternalFormat = GL_RGB8;
		mDataFormat     = GL_RGB;
	}
	else
		ASSERT(false, "Channel count not supported");

	glCreateTextures(GL_TEXTURE_2D, 1, &mID);
	glTextureStorage2D(mID, 1, mInternalFormat, mWidth, mHeight);
	glTextureSubImage2D(mID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE,
	                    raw);
	SetFilter(false);
	SetWrapMode(WrapMode::Repeat);

	stbi_image_free(raw);
}

TextureGL::TextureGL(uword width, uword height, bool filter, WrapMode wrap,
                     Color border)
        : mWidth(width), mHeight(height), mDataFormat(GL_RGBA),
          mInternalFormat(GL_RGBA8)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &mID);
	glTextureStorage2D(mID, 1, mInternalFormat, width, height);
	SetFilter(filter);
	SetWrapMode(wrap, border);
}

TextureGL::~TextureGL()
{
	glDeleteTextures(1, &mID);
}

void TextureGL::Bind(uword slot) const
{
	glBindTextureUnit(slot, mID);
}

size_t TextureGL::GetSize() const
{
	uword bpp = mDataFormat == GL_RGBA ? 4 : 3;
	return size_t(mWidth) * mHeight * bpp;
}

Vec2ui TextureGL::GetResolution() const
{
	return Vec2i {mWidth, mHeight};
}

uword TextureGL::GetWidth() const
{
	return mWidth;
}

uword TextureGL::GetHeight() const
{
	return mHeight;
}

bool TextureGL::IsFiltered() const
{
	return mFiltered;
}

void TextureGL::SetFilter(bool enable)
{
	mFiltered = enable;

	if(enable)
	{
		glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

WrapMode TextureGL::GetWrapMode() const
{
	return mWrapMode;
}

void TextureGL::SetWrapMode(WrapMode wrap, Color border)
{
	mWrapMode = wrap;
	mBorder   = border;

	switch(wrap)
	{
	case WrapMode::Repeat:
		glTextureParameteri(mID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;

	case WrapMode::Mirror:
		glTextureParameteri(mID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTextureParameteri(mID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;

	case WrapMode::Border:
	{
		float t[4];
		border.GetColors(t);
		glTextureParameteri(mID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTextureParameteri(mID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTextureParameterfv(mID, GL_TEXTURE_BORDER_COLOR, t);
	}
	break;

	case WrapMode::Clamp:
		glTextureParameteri(mID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(mID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	}
}

void TextureGL::SetData(const void* data, size_t size)
{
	ASSERT(size == GetSize(), "Incorrect texture size");
	glTextureSubImage2D(mID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE,
	                    data);
}

uword TextureGL::GetID() const
{
	return mID;
}
