#pragma once

#include <Texture.hpp>

#include <glad/gl.h>

class TextureGL final: public Texture
{
public:
	TextureGL();
	TextureGL(const Path& path);
	TextureGL(U32 width, U32 height, bool filter = false,
	          WrapMode wrap = WrapMode::Repeat, Color border = Color::WHITE);
	~TextureGL();

	void Bind(U32 slot) const override;

	size_t GetSize() const override;

	Vec2ui GetResolution() const override;
	U32    GetWidth() const override;
	U32    GetHeight() const override;

	bool IsFiltered() const override;
	void SetFilter(bool enable) override;

	WrapMode GetWrapMode() const override;
	void     SetWrapMode(WrapMode wrap, Color border = Color::WHITE) override;

	void SetData(const void* data, size_t size) override;

	U32 GetID() const override;

private:
	U32      mID;
	U32      mWidth;
	U32      mHeight;
	bool     mFiltered;
	WrapMode mWrapMode;
	Color    mBorder;
	U32      mDataFormat;
	U32      mInternalFormat;
};
