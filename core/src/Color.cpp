#include <Color.hpp>

const Color Color::BLANK {0x00000000};
const Color Color::WHITE {0xffffffff};
const Color Color::BLACK {0x000000ff};
const Color Color::RED {0xff0000ff};
const Color Color::GREEN {0x00ff00ff};
const Color Color::BLUE {0x0000ffff};
const Color Color::YELLOW {0xffff00ff};
const Color Color::CYAN {0x00ffffff};
const Color Color::MAGENTA {0xff00ffff};
const Color Color::BROWN {0x964B00ff};
const Color Color::PURPLE {0xa020f0ff};

const Color Color::ALICE_BLUE {0xf0f8ffff};
const Color Color::ANTIQUE_WHITE {0xfaebd7ff};
const Color Color::AQUA {0x00ffffff};
const Color Color::AQUAMARINE {0x7fffd4ff};
const Color Color::AZURE {0x007fffff};
const Color Color::BLUE_VIOLET {0x8a2be2ff};
const Color Color::BURLYWOOD {0xdeb887ff};
const Color Color::CADET_BLUE {0x5f9ea0ff};
const Color Color::CHARTREUSE {0xdfff00ff};
const Color Color::CHOCOLATE {0x7b3f00ff};
const Color Color::CORNFLOWER_BLUE {0x6495edff};
const Color Color::CORNSILK {0xfff8dcff};
const Color Color::CRIMSON {0xdc143cff};
const Color Color::DARK_ORCHID {0x9932ccff};
const Color Color::DARK_SALMON {0xe9967aff};
const Color Color::DEEP_PINK {0xaa336aff};
const Color Color::FLORAL_WHITE {0xfffaf0ff};
const Color Color::GHOST_WHITE {0xf8f8ffff};
const Color Color::GOLD {0xffd700ff};
const Color Color::INDIGO {0x4b0082ff};
const Color Color::IVORY {0xfffff0ff};
const Color Color::LAVENDER {0xe6e6faff};
const Color Color::LAWN_GREEN {0x7cfc00ff};
const Color Color::LIGHT_CORAL {0xf08080ff};
const Color Color::MOCCASIN {0xffe4b5ff};
const Color Color::PLUM {0x673147ff};
const Color Color::POWDER_BLUE {0xb6d0e2ff};
const Color Color::ROYAL_BLUE {0x4169e1ff};
const Color Color::ROSY_BROWN {0xbc8f8fff};
const Color Color::SLATE_BLUE {0x6a5acdff};
const Color Color::SNOW {0xfffafaff};
const Color Color::TEAL {0x008080ff};
const Color Color::TURQUOISE {0x30d5c8ff};
const Color Color::WHITE_SMOKE {0xf5f5f5ff};
const Color Color::YELLOW_GREEN {0x9acd32ff};

void Color::Set(u8 r, u8 g, u8 b, u8 a)
{
	mColor = static_cast<u32>(a << 24u) | static_cast<u32>(b << 16u) |
	         static_cast<u32>(g << 8u) | static_cast<u32>(r);
}

void Color::Set(float r, float g, float b, float a)
{
	mColor = static_cast<u8>(a * 255u) | static_cast<u8>(b * 255u) |
	         static_cast<u8>(g * 255u) | static_cast<u8>(r * 255u);
}

void Color::Set(u8 r, u8 g, u8 b)
{
	Set(r, g, b, 255);
}

void Color::Set(float r, float g, float b)
{
	Set(r, g, b, 1.0f);
}

void Color::SetRed(u8 r)
{
	mColor = (mColor & 0xffffff00) | static_cast<u32>(r);
}

void Color::SetGreen(u8 g)
{
	mColor = (mColor & 0xffff00ff) | (static_cast<u32>(g) << 8u);
}

void Color::SetBlue(u8 b)
{
	mColor = (mColor & 0xff00ffff) | (static_cast<u32>(b) << 16u);
}

void Color::SetAlpha(u8 a)
{
	mColor = (mColor & 0x00ffffff) | (static_cast<u32>(a) << 24u);
}

void Color::SetRed(float r)
{
	SetRed(static_cast<u8>(r * 255));
}

void Color::SetGreen(float g)
{
	SetGreen(static_cast<u8>(g * 255));
}

void Color::SetBlue(float b)
{
	SetBlue(static_cast<u8>(b * 255));
}

void Color::SetAlpha(float a)
{
	SetAlpha(static_cast<u8>(a * 255));
}

void Color::SetPacked(u32 pack)
{
	mColor = ((pack >> 24) & 0xff) | ((pack << 8) & 0xff0000) |
	         ((pack >> 8) & 0xff00) | ((pack << 24) & 0xff000000);
}

void Color::GetColors(float* out) const
{
	out[0] = static_cast<float>(GetRed()) / 255.0f;
	out[1] = static_cast<float>(GetGreen()) / 255.0f;
	out[2] = static_cast<float>(GetBlue()) / 255.0f;
	out[3] = static_cast<float>(GetAlpha()) / 255.0f;
}

bool Color::operator==(Color rhs) const
{
	return mColor == rhs.mColor;
}
