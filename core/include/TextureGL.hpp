#pragma once

#include <Common.hpp>
#include <Texture.hpp>

class TextureGL final: public Texture
{
public:
	TextureGL();
	explicit TextureGL(const Path& path);
	TextureGL(u32      width,
	          u32      height,
	          bool     filter = false,
	          WrapMode wrap   = WrapMode::Repeat,
	          Color    border = Color::WHITE);
	~TextureGL() override;

	void Bind(u32 slot) const override;

	size_t GetSize() const override;

	vec2ui GetResolution() const override;
	u32    GetWidth() const override;
	u32    GetHeight() const override;

	bool IsFiltered() const override;
	void SetFilter(bool enable) override;

	WrapMode GetWrapMode() const override;
	void     SetWrapMode(WrapMode wrap, Color border = Color::WHITE) override;

	void SetData(const void* data, size_t size) override;

	u32 GetID() const override;

private:
	u32      mID {0};
	u32      mWidth {1};
	u32      mHeight {1};
	bool     mFiltered {false};
	WrapMode mWrapMode {WrapMode::Repeat};
	Color    mBorder {Color::WHITE};
	u32      mDataFormat {0};
	u32      mInternalFormat {0};
};
