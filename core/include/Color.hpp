#pragma once

#include <Common.hpp>

// 32-bit packed color in ABGR layout
struct Color
{
private:
	u32 mColor {0u};

public:
	constexpr Color()              = default;
	constexpr Color(const Color&)  = default;
	constexpr Color(Color&&)       = default;
	Color& operator=(const Color&) = default;
	Color& operator=(Color&&)      = default;

	constexpr Color(u32 color)
	        : mColor(((color >> 24) & 0xff) | ((color << 8) & 0xff0000) |
	                 ((color >> 8) & 0xff00) | ((color << 24) & 0xff000000))
	{
	}

	constexpr Color(u8 r, u8 g, u8 b, u8 a)
	        : mColor(static_cast<u32>(a << 24u) | static_cast<u32>(b << 16u) |
	                 static_cast<u32>(g << 8u) | static_cast<u32>(r))
	{
	}

	constexpr Color(float r, float g, float b, float a)
	        : Color(static_cast<u8>(a * 255u),
	                static_cast<u8>(b * 255u),
	                static_cast<u8>(g * 255u),
	                static_cast<u8>(r * 255u))
	{
	}

	constexpr Color(u8 r, u8 g, u8 b)
	        : Color(r, g, b, 255)
	{
	}

	constexpr Color(float r, float g, float b)
	        : Color(r, g, b, 1.0f)
	{
	}

	constexpr Color(Color color, u8 alpha)
	        : mColor((static_cast<u32>(alpha) << 24) | color.mColor)
	{
	}

	constexpr Color(Color color, float alpha)
	        : Color(color, static_cast<u8>(alpha * 255u))
	{
	}

	void Set(u8 r, u8 g, u8 b, u8 a);
	void Set(float r, float g, float b, float a);
	void Set(u8 r, u8 g, u8 b);
	void Set(float r, float g, float b);

	void         GetColors(float* out) const;
	constexpr u8 GetRed() const
	{
		return static_cast<u8>(mColor);
	}

	constexpr u8 GetGreen() const
	{
		return static_cast<u8>(mColor >> 8u);
	}

	constexpr u8 GetBlue() const
	{
		return static_cast<u8>(mColor >> 16u);
	}

	constexpr u8 GetAlpha() const
	{
		return static_cast<u8>(mColor >> 24u);
	}

	void SetRed(u8 r);
	void SetGreen(u8 g);
	void SetBlue(u8 b);
	void SetAlpha(u8 a);

	void SetRed(float r);
	void SetGreen(float g);
	void SetBlue(float b);
	void SetAlpha(float a);

	void          SetPacked(u32 pack);
	constexpr u32 GetPacked() const
	{
		return mColor;
	}

	bool operator==(Color rhs) const;
	bool operator!=(Color rhs) const
	{
		return !operator==(rhs);
	}

public:
	static const Color BLANK;
	static const Color WHITE;
	static const Color BLACK;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color YELLOW;
	static const Color CYAN;
	static const Color MAGENTA;
	static const Color BROWN;
	static const Color PURPLE;

	static const Color ALICE_BLUE;
	static const Color ANTIQUE_WHITE;
	static const Color AQUA;
	static const Color AQUAMARINE;
	static const Color AZURE;
	static const Color BLUE_VIOLET;
	static const Color BURLYWOOD;
	static const Color CADET_BLUE;
	static const Color CHARTREUSE;
	static const Color CHOCOLATE;
	static const Color CORNFLOWER_BLUE;
	static const Color CORNSILK;
	static const Color CRIMSON;
	static const Color DARK_ORCHID;
	static const Color DARK_SALMON;
	static const Color DEEP_PINK;
	static const Color FLORAL_WHITE;
	static const Color GHOST_WHITE;
	static const Color GOLD;
	static const Color INDIGO;
	static const Color IVORY;
	static const Color LAVENDER;
	static const Color LAWN_GREEN;
	static const Color LIGHT_CORAL;
	static const Color MOCCASIN;
	static const Color PLUM;
	static const Color POWDER_BLUE;
	static const Color ROYAL_BLUE;
	static const Color ROSY_BROWN;
	static const Color SLATE_BLUE;
	static const Color SNOW;
	static const Color TEAL;
	static const Color TURQUOISE;
	static const Color WHITE_SMOKE;
	static const Color YELLOW_GREEN;
};
