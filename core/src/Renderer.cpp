#include <Renderer.hpp>

#include <Logger.hpp>

Renderer::Renderer(RenderDevicePtr& device, uword max_quads)
        : mDevice(*device), mMaxQuads(max_quads), mMaxVertices(mMaxQuads * 4),
          mMaxIndices(mMaxQuads * 6), mTextureIndex(0), mStats {},
          mQuadVertices(mMaxVertices), mQuadCount(0), mCircleVertices(mMaxVertices),
          mCircleCount(0)
{
	TRACE("Renderer initializing...");

	{  // Create index buffer and upload to GPU
		Vector<uword> indices(mMaxIndices);
		for(uword offset = 0, i = 0; i < mMaxIndices; i += 6, offset += 4)
		{
			indices[i]     = offset;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset;
		}

		mIB = IndexBuffer::Create(indices.data(), mMaxIndices);
	}

	// Quad initialization
	mQuadVB = VertexBuffer::Create(
	        {Vertex {VertexType::Float2}, Vertex {VertexType::Float2},
	         Vertex {VertexType::UByte4, true}, Vertex {VertexType::Float}},
	        mMaxVertices, sizeof(QuadVertex));

	mQuadVA = VertexArray::Create();
	mQuadVA->AttachIndexBuffer(mIB);
	mQuadVA->AttachVertexBuffer(mQuadVB);
	mQuadShader = Shader::Create("shaders/QuadShader.glsl");

	// Circle initialization
	mCircleVB = VertexBuffer::Create(
	        {Vertex {VertexType::Float2}, Vertex {VertexType::Float2},
	         Vertex {VertexType::UByte4, true}, Vertex {VertexType::Float},
	         Vertex {VertexType::Float}},
	        mMaxVertices, sizeof(CircleVertex));

	mCircleVA = VertexArray::Create();
	mCircleVA->AttachIndexBuffer(mIB);
	mCircleVA->AttachVertexBuffer(mCircleVB);
	mCircleShader = Shader::Create("shaders/CircleShader.glsl");

	// Textures initialization
	mWhiteTexture = Texture::Create();
	mTextures.resize(mDevice.GetInfo().NumTextureUnits);
	mTextures[mTextureIndex++] = mWhiteTexture;

	TRACE("Renderer initialized");
}

Renderer::~Renderer()
{
	TRACE("Renderer destroying...");

	TRACE("Renderer destroyed");
}

const FrameStats& Renderer::GetFrameStats()
{
	return mStats;
}

void Renderer::DrawBegin(Camera& camera)
{
	mViewProjection = camera.GetViewProjection();

	BatchBegin();

	mStats.DrawCalls = 0;
	mStats.QuadCount = 0;
}

void Renderer::DrawEnd()
{
	BatchEnd();
}

void Renderer::BatchBegin()
{
	mQuadCount   = 0;
	mCircleCount = 0;

	mTextureIndex = 1;
}

void Renderer::BatchEnd()
{
	// flush quads
	if(mQuadCount != 0)
	{
		for(uword i = 0; i < mTextureIndex; ++i) { mTextures[i]->Bind(i); }

		mQuadVB->SetData(mQuadVertices.data(), mQuadCount * 4);
		mQuadShader->Bind();
		mQuadShader->SetTransform("uViewProjection", mViewProjection);
		mDevice.DrawIndexed(mQuadVA, mQuadCount * 6);
		++mStats.DrawCalls;
	}

	// flush circles
	if(mCircleCount != 0)
	{
		mCircleVB->SetData(mCircleVertices.data(), mCircleCount * 4);
		mCircleShader->Bind();
		mCircleShader->SetTransform("uViewProjection", mViewProjection);
		mDevice.DrawIndexed(mCircleVA, mCircleCount * 6);
		++mStats.DrawCalls;
	}
}

void Renderer::DrawQuad(Transform& model, const TexturePtr& texture,
                        Color tint_color, const Vec2 tex_coords[])
{
	if(mQuadCount >= mMaxQuads)
	{
		BatchEnd();
		BatchBegin();
	}

	float texture_index = 0.0f;
	for(uword i = 1; i < mTextureIndex; ++i)
	{
		if(*texture == *mTextures[i])
		{
			texture_index = static_cast<float>(i);
			break;
		}
	}

	if(texture_index == 0.0f)
	{
		if(mTextureIndex > mDevice.GetInfo().NumTextureUnits - 1)
		{
			BatchEnd();
			BatchBegin();
		}

		texture_index = static_cast<float>(mTextureIndex);

		mTextures[mTextureIndex++] = texture;
	}

	uword i = mQuadCount * 4;

	mQuadVertices[i].Position = model * mQuadPositions[0];
	mQuadVertices[i].UV       = tex_coords[0];
	mQuadVertices[i].Color    = tint_color;
	mQuadVertices[i++].TexID  = texture_index;

	mQuadVertices[i].Position = model * mQuadPositions[1];
	mQuadVertices[i].UV       = tex_coords[1];
	mQuadVertices[i].Color    = tint_color;
	mQuadVertices[i++].TexID  = texture_index;

	mQuadVertices[i].Position = model * mQuadPositions[2];
	mQuadVertices[i].UV       = tex_coords[2];
	mQuadVertices[i].Color    = tint_color;
	mQuadVertices[i++].TexID  = texture_index;

	mQuadVertices[i].Position = model * mQuadPositions[3];
	mQuadVertices[i].UV       = tex_coords[3];
	mQuadVertices[i].Color    = tint_color;
	mQuadVertices[i].TexID    = texture_index;

	++mQuadCount;
	++mStats.QuadCount;
}

void Renderer::DrawQuad(Vec2 pos, Vec2 size, const TexturePtr& texture,
                        Color tint_color, const Vec2 tex_coords[])
{
	Transform model;
	model.Translate(pos).Scale(size);
	DrawQuad(model, texture, tint_color, tex_coords);
}

void Renderer::DrawQuad(Vec2 pos, Vec2 size, Color color)
{
	Transform model;
	model.Translate(pos).Scale(size);
	DrawQuad(model, mWhiteTexture, color);
}

void Renderer::DrawRotatedQuad(Vec2 pos, Vec2 size, float angle,
                               const TexturePtr& texture, Color tint_color,
                               const Vec2 tex_coords[])
{
	Transform model;
	model.Translate(pos).Rotate(angle).Scale(size);
	DrawQuad(model, texture, tint_color, tex_coords);
}

void Renderer::DrawRotatedQuad(Vec2 pos, Vec2 size, float angle, Color color)
{
	Transform model;
	model.Translate(pos).Rotate(angle).Scale(size);
	DrawQuad(model, mWhiteTexture, color);
}

void Renderer::DrawCircle(Vec2 pos, float radius, Color color, float thickness,
                          float smoothness)
{
	if(mCircleCount >= mMaxQuads)
	{
		BatchEnd();
		BatchBegin();
	}

	Transform model;
	model.Translate(pos).Scale(radius);

	uword i = mCircleCount * 4;

	mCircleVertices[i].WorldPosition = model * mQuadPositions[0];
	mCircleVertices[i].LocalPosition = mCirclePositions[0];
	mCircleVertices[i].Color         = color;
	mCircleVertices[i].Thickness     = thickness;
	mCircleVertices[i++].Smoothness  = smoothness;

	mCircleVertices[i].WorldPosition = model * mQuadPositions[1];
	mCircleVertices[i].LocalPosition = mCirclePositions[1];
	mCircleVertices[i].Color         = color;
	mCircleVertices[i].Thickness     = thickness;
	mCircleVertices[i++].Smoothness  = smoothness;

	mCircleVertices[i].WorldPosition = model * mQuadPositions[2];
	mCircleVertices[i].LocalPosition = mCirclePositions[2];
	mCircleVertices[i].Color         = color;
	mCircleVertices[i].Thickness     = thickness;
	mCircleVertices[i++].Smoothness  = smoothness;

	mCircleVertices[i].WorldPosition = model * mQuadPositions[3];
	mCircleVertices[i].LocalPosition = mCirclePositions[3];
	mCircleVertices[i].Color         = color;
	mCircleVertices[i].Thickness     = thickness;
	mCircleVertices[i].Smoothness    = smoothness;

	++mCircleCount;
	++mStats.QuadCount;
}
