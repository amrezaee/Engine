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
	uword DrawCalls;
	uword QuadCount;
};

struct QuadVertex
{
	Vec2  Position;
	Vec2  UV;
	Color Color;
	float TexID;
};

struct CircleVertex
{
	Vec2  WorldPosition;
	Vec2  LocalPosition;
	Color Color;
	float Thickness;
	float Smoothness;
};

class Renderer
{
public:
	Renderer(RenderDevice& device, uword max_quads);
	Renderer(const Renderer&)            = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();

	void Initialize();
	void Destroy();

	const FrameStats& GetFrameStats();

	void DrawBegin(Camera& camera);
	void DrawEnd();
	void BatchBegin();
	void BatchEnd();

	void DrawQuad(Transform& model, const TexturePtr& texture,
	              Color      tint_color   = Color::WHITE,
	              const Vec2 tex_coords[] = mTextureCoords);

	void DrawQuad(Vec2 pos, Vec2 size, const TexturePtr& texture,
	              Color      tint_color   = Color::WHITE,
	              const Vec2 tex_coords[] = mTextureCoords);

	void DrawQuad(Vec2 pos, Vec2 size, Color color);

	void DrawRotatedQuad(Vec2 pos, Vec2 size, float angle, const TexturePtr& texture,
	                     Color      tint_color   = Color::WHITE,
	                     const Vec2 tex_coords[] = mTextureCoords);

	void DrawRotatedQuad(Vec2 pos, Vec2 size, float angle, Color color);

	void DrawCircle(Vec2 pos, float radius, Color color, float thickness = 1.0f,
	                float smoothness = 0.03f);

private:
	RenderDevice& mDevice;

	const uword mMaxQuads;
	const uword mMaxVertices;
	const uword mMaxIndices;

	IndexBufferPtr     mIB;
	Vector<TexturePtr> mTextures;
	uword              mTextureIndex;
	TexturePtr         mWhiteTexture;
	Transform          mViewProjection;
	FrameStats         mStats;

	VertexBufferPtr    mQuadVB;
	VertexArrayPtr     mQuadVA;
	Vector<QuadVertex> mQuadVertices;
	ShaderPtr          mQuadShader;
	uword              mQuadCount;

	VertexBufferPtr      mCircleVB;
	VertexArrayPtr       mCircleVA;
	Vector<CircleVertex> mCircleVertices;
	ShaderPtr            mCircleShader;
	uword                mCircleCount;

	constexpr static Vec2 mQuadPositions[4] = {
	        {-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f}};

	constexpr static Vec2 mTextureCoords[4] = {
	        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
};
