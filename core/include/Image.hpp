#pragma once

#include <Common.hpp>

#include <Color.hpp>

class Image
{
public:
	Image(): mPixels(2, Color::WHITE), mWidth(1), mHeight(1) {}
	Image(uint32_t width, uint32_t height)
	        : mPixels(width * height, Color::WHITE), mWidth(width), mHeight(height)
	{
	}

	Image(uint32_t width, uint32_t height, const Color* data)
	        : mPixels(data, data + width * height), mWidth(width), mHeight(height)
	{
	}

	Color GetColor(uint32_t x, uint32_t y) const
	{
		return mPixels.at(y * mWidth + x);
	}
	void SetColor(uint32_t x, uint32_t y, Color color)
	{
		mPixels.at(y * mWidth + x) = color;
	}
	size_t   GetSize() const { return mWidth * mHeight * sizeof(Color); }
	uint32_t GetWidth() const { return mWidth; }
	uint32_t GetHeight() const { return mHeight; }
	void     SetData(uint32_t width, uint32_t height, const Color* data)
	{
		size_t size = (size_t)width * height;
		if(width != mWidth || height != mHeight)
		{
			mWidth  = width;
			mHeight = height;
			mPixels.resize(size);
		}
		std::copy(data, data + size, mPixels.begin());
	}
	const Color* GetData() const { return mPixels.data(); }
	Color*       GetData() { return mPixels.data(); }

private:
	std::vector<Color> mPixels;
	uint32_t           mWidth;
	uint32_t           mHeight;
};
