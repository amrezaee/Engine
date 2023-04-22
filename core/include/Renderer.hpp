#pragma once

#include <Camera.hpp>
#include <Color.hpp>
#include <Common.hpp>
#include <GPUBuffers.hpp>
#include <RenderDevice.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <Transform.hpp>
#include <Vector2.hpp>

class Window;

struct FrameStats
{
	u32 DrawCalls;
	u32 QuadCount;
};

struct QuadVertex
{
	vec2  Position;
	vec2  UV;
	Color Color;
	float TexID;
};

struct CircleVertex
{
	vec2  WorldPosition;
	vec2  LocalPosition;
	Color Color;
	float Thickness;
	float Smoothness;
};

class Renderer
{
public:
	Renderer(RenderDevicePtr& device, u32 max_quads);
	Renderer(const Renderer&)            = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();

	void DrawBegin(const Transform& view_projection);
	void DrawEnd();
	void Flush();

	const FrameStats& GetFrameStats() const
	{
		return mStats;
	}

	void SetColor(Color color)
	{
		mColor = color;
	}
	void SetColor(float r, float g, float b, float a)
	{
		mColor.Set(r, g, b, a);
	}
	Color GetColor() const
	{
		return mColor;
	}

	void DrawQuad(const TexturePtr& texture, const Transform& model, const vec2* uv);
	void DrawQuad(const TexturePtr& texture, vec2 position, vec2 size);
	void DrawQuad(vec2 position, vec2 size);

	void DrawRotatedQuad(const TexturePtr& texture,
	                     vec2              position,
	                     vec2              size,
	                     float             rotation,
	                     vec2              origin);
	void DrawRotatedQuad(const TexturePtr& texture,
	                     vec2              position,
	                     vec2              size,
	                     float             rotation);
	void DrawRotatedQuad(vec2 position, vec2 size, float rotation, vec2 origin);
	void DrawRotatedQuad(vec2 position, vec2 size, float rotation);

	void DrawCircle(vec2  position,
	                float radius,
	                float thickness  = 1.0f,
	                float smoothness = 0.03f);

private:
	RenderDevice& mDevice;

	Color      mColor;
	FrameStats mStats;

	const u32 mMaxQuads;
	const u32 mMaxVertices;
	const u32 mMaxIndices;

	IndexBufferPtr     mIB;
	Vector<TexturePtr> mTextures;
	u32                mTextureIndex;
	TexturePtr         mWhiteTexture;
	Transform          mViewProjection;

	VertexBufferPtr    mQuadVB;
	VertexArrayPtr     mQuadVA;
	Vector<QuadVertex> mQuadVertices;
	ShaderPtr          mQuadShader;
	u32                mQuadCount;

	VertexBufferPtr      mCircleVB;
	VertexArrayPtr       mCircleVA;
	Vector<CircleVertex> mCircleVertices;
	ShaderPtr            mCircleShader;
	u32                  mCircleCount;

	const vec2 mQuadPositions[4] = {
	        {-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f}};

	const vec2 mCirclePositions[4] = {
	        {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}};

	const vec2 mTextureUV[4] = {
	        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
};
