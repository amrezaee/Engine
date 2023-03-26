#pragma once

#include <Common.hpp>
#include <Texture.hpp>

class TextureGL final: public Texture
{
public:
	TextureGL();
	explicit TextureGL(const Path& path);
	TextureGL(uword width, uword height, bool filter = false,
	          WrapMode wrap = WrapMode::Repeat, Color border = Color::WHITE);
	~TextureGL() override;

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
	uword    mID {0};
	uword    mWidth {1};
	uword    mHeight {1};
	bool     mFiltered {false};
	WrapMode mWrapMode {WrapMode::Repeat};
	Color    mBorder {Color::WHITE};
	uword    mDataFormat {0};
	uword    mInternalFormat {0};
};
