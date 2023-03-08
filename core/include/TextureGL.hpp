#pragma once

#include <Common.hpp>
#include <Texture.hpp>

class TextureGL final: public Texture
{
public:
	TextureGL();
	TextureGL(const Path& path);
	TextureGL(uword width, uword height, bool filter = false,
	          WrapMode wrap = WrapMode::Repeat, Color border = Color::WHITE);
	~TextureGL();

	void Bind(uword slot) const override;

	size_t GetSize() const override;

	Vec2ui GetResolution() const override;
	uword  GetWidth() const override;
	uword  GetHeight() const override;

	bool IsFiltered() const override;
	void SetFilter(bool enable) override;

	WrapMode GetWrapMode() const override;
	void     SetWrapMode(WrapMode wrap, Color border = Color::WHITE) override;

	void SetData(const void* data, size_t size) override;

	uword GetID() const override;

private:
	uword    mID;
	uword    mWidth;
	uword    mHeight;
	bool     mFiltered;
	WrapMode mWrapMode;
	Color    mBorder;
	uword    mDataFormat;
	uword    mInternalFormat;
};
