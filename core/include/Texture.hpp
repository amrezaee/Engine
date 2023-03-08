#pragma once

#include <Color.hpp>
#include <Common.hpp>
#include <Vector2.hpp>

class Texture;
using TexturePtr = SharedPtr<Texture>;

enum class WrapMode
{
	Repeat,
	Mirror,
	Border,
	Clamp
};

class Texture
{
public:
	virtual ~Texture() = default;

	static TexturePtr Create();
	static TexturePtr Create(const Path& path);
	static TexturePtr Create(uword width, uword height, bool filter = false,
	                         WrapMode wrap   = WrapMode::Repeat,
	                         Color    border = Color::WHITE);

	virtual void Bind(uword slot) const = 0;

	virtual size_t GetSize() const = 0;  // size in bytes

	virtual Vec2ui GetResolution() const = 0;
	virtual uword  GetWidth() const      = 0;
	virtual uword  GetHeight() const     = 0;

	virtual bool IsFiltered() const     = 0;
	virtual void SetFilter(bool enable) = 0;

	virtual WrapMode GetWrapMode() const                                     = 0;
	virtual void     SetWrapMode(WrapMode wrap, Color border = Color::WHITE) = 0;

	virtual void SetData(const void* data, size_t size) = 0;

	virtual uword GetID() const = 0;

	bool operator==(const Texture& rhs) const;
	bool operator!=(const Texture& rhs) const;
};
