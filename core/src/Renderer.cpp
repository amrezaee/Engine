#include <Renderer.hpp>

#include <Logger.hpp>

Renderer::Renderer(RenderDevicePtr& device, u32 max_quads)
        : mDevice {*device},
          mColor {Color::WHITE},
          mStats {},
          mMaxQuads {max_quads},
          mMaxVertices {mMaxQuads * 4},
          mMaxIndices {mMaxQuads * 6},
          mTextureIndex {},
          mQuadVertices {mMaxVertices},
          mQuadCount {},
          mCircleVertices {mMaxVertices},
          mCircleCount {}
{
	TRACE("Renderer initializing...");

	{  // Create index buffer and upload to GPU
		Vector<u32> indices(mMaxIndices);
		for(u32 offset = 0, i = 0; i < mMaxIndices; i += 6, offset += 4)
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

	mQuadVB = VertexBuffer::Create({Vertex {VertexType::Float2},
	                                Vertex {VertexType::Float2},
	                                Vertex {VertexType::UByte4, true},
	                                Vertex {VertexType::Float}},
	                               mMaxVertices,
	                               sizeof(QuadVertex));

	mQuadVA = VertexArray::Create();
	mQuadVA->AttachIndexBuffer(mIB);
	mQuadVA->AttachVertexBuffer(mQuadVB);
	mQuadShader = Shader::Create("shaders/QuadShader.glsl");

	mCircleVB = VertexBuffer::Create({Vertex {VertexType::Float2},
	                                  Vertex {VertexType::Float2},
	                                  Vertex {VertexType::UByte4, true},
	                                  Vertex {VertexType::Float},
	                                  Vertex {VertexType::Float}},
	                                 mMaxVertices,
	                                 sizeof(CircleVertex));

	mCircleVA = VertexArray::Create();
	mCircleVA->AttachIndexBuffer(mIB);
	mCircleVA->AttachVertexBuffer(mCircleVB);
	mCircleShader = Shader::Create("shaders/CircleShader.glsl");

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

void Renderer::DrawBegin(const Transform& view_projection)
{
	mViewProjection = view_projection;

	mQuadCount    = 0;
	mCircleCount  = 0;
	mTextureIndex = 1;

	mStats.DrawCalls = 0;
	mStats.QuadCount = 0;
}

void Renderer::DrawEnd()
{
	Flush();
}

void Renderer::Flush()
{
	// flush quads
	if(mQuadCount != 0)
	{
		for(u32 i = 0; i < mTextureIndex; ++i)
		{
			mTextures[i]->Bind(i);
		}

		mQuadVB->SetData(mQuadVertices.data(), mQuadCount * 4);
		mQuadShader->Bind();
		mQuadShader->SetTransform("uViewProjection", mViewProjection);
		mDevice.DrawIndexed(mQuadVA, mQuadCount * 6);
		++mStats.DrawCalls;
		mQuadCount    = 0;
		mTextureIndex = 1;
	}

	// flush circles
	if(mCircleCount != 0)
	{
		mCircleVB->SetData(mCircleVertices.data(), mCircleCount * 4);
		mCircleShader->Bind();
		mCircleShader->SetTransform("uViewProjection", mViewProjection);
		mDevice.DrawIndexed(mCircleVA, mCircleCount * 6);
		++mStats.DrawCalls;
		mCircleCount = 0;
	}
}

void Renderer::DrawQuad(const TexturePtr& texture,
                        const Transform&  model,
                        const vec2*       uv)
{
	if(mQuadCount >= mMaxQuads)
	{
		Flush();
	}

	u32 index = 0;
	while(index < mTextureIndex)
	{
		if(*texture == *mTextures[index++])
		{
			break;  // texture does found!
		}
	}
	// texture does not found!
	if(index == mTextureIndex)
	{
		// texture slots are full. so, Flush!
		if(mTextureIndex == mDevice.GetInfo().NumTextureUnits)
		{
			Flush();
		}
		// now add new texture.
		index                      = mTextureIndex;
		mTextures[mTextureIndex++] = texture;
	}

	u32 i = mQuadCount * 4;

	mQuadVertices[i].Position = model * mQuadPositions[0];
	mQuadVertices[i].UV       = uv[0];
	mQuadVertices[i].Color    = mColor;
	mQuadVertices[i++].TexID  = static_cast<float>(index);

	mQuadVertices[i].Position = model * mQuadPositions[1];
	mQuadVertices[i].UV       = uv[1];
	mQuadVertices[i].Color    = mColor;
	mQuadVertices[i++].TexID  = static_cast<float>(index);

	mQuadVertices[i].Position = model * mQuadPositions[2];
	mQuadVertices[i].UV       = uv[2];
	mQuadVertices[i].Color    = mColor;
	mQuadVertices[i++].TexID  = static_cast<float>(index);

	mQuadVertices[i].Position = model * mQuadPositions[3];
	mQuadVertices[i].UV       = uv[3];
	mQuadVertices[i].Color    = mColor;
	mQuadVertices[i].TexID    = static_cast<float>(index);

	++mQuadCount;
	++mStats.QuadCount;
}

void Renderer::DrawQuad(const TexturePtr& texture, vec2 position, vec2 size)
{
	Transform model;
	model.Translate(position).Scale(size);
	DrawQuad(texture, model, mTextureUV);
}

void Renderer::DrawQuad(vec2 position, vec2 size)
{
	Transform model;
	model.Translate(position).Scale(size);
	DrawQuad(mWhiteTexture, model, mTextureUV);
}

void Renderer::DrawRotatedQuad(const TexturePtr& texture,
                               vec2              position,
                               vec2              size,
                               float             rotation,
                               vec2              origin)
{
	Transform model;
	model.Translate(position)
	        .Translate(origin)
	        .Rotate(rotation)
	        .Translate(-origin)
	        .Scale(size);
	DrawQuad(texture, model, mTextureUV);
}

void Renderer::DrawRotatedQuad(const TexturePtr& texture,
                               vec2              position,
                               vec2              size,
                               float             rotation)
{
	Transform model;
	model.Translate(position).Rotate(rotation).Scale(size);
	DrawQuad(texture, model, mTextureUV);
}

void Renderer::DrawRotatedQuad(vec2 position, vec2 size, float rotation, vec2 origin)
{
	Transform model;
	model.Translate(position)
	        .Translate(origin)
	        .Rotate(rotation)
	        .Translate(-origin)
	        .Scale(size);
	DrawQuad(mWhiteTexture, model, mTextureUV);
}

void Renderer::DrawRotatedQuad(vec2 position, vec2 size, float rotation)
{
	Transform model;
	model.Translate(position).Rotate(rotation).Scale(size);
	DrawQuad(mWhiteTexture, model, mTextureUV);
}

void Renderer::DrawCircle(vec2  position,
                          float radius,
                          float thickness,
                          float smoothness)
{
	if(mCircleCount >= mMaxQuads)
	{
		Flush();
	}

	Transform model;
	model.Translate(position).Scale(radius);

	u32 i = mCircleCount * 4;

	mCircleVertices[i].WorldPosition = model * mQuadPositions[0];
	mCircleVertices[i].LocalPosition = mCirclePositions[0];
	mCircleVertices[i].Color         = mColor;
	mCircleVertices[i].Thickness     = thickness;
	mCircleVertices[i++].Smoothness  = smoothness;

	mCircleVertices[i].WorldPosition = model * mQuadPositions[1];
	mCircleVertices[i].LocalPosition = mCirclePositions[1];
	mCircleVertices[i].Color         = mColor;
	mCircleVertices[i].Thickness     = thickness;
	mCircleVertices[i++].Smoothness  = smoothness;

	mCircleVertices[i].WorldPosition = model * mQuadPositions[2];
	mCircleVertices[i].LocalPosition = mCirclePositions[2];
	mCircleVertices[i].Color         = mColor;
	mCircleVertices[i].Thickness     = thickness;
	mCircleVertices[i++].Smoothness  = smoothness;

	mCircleVertices[i].WorldPosition = model * mQuadPositions[3];
	mCircleVertices[i].LocalPosition = mCirclePositions[3];
	mCircleVertices[i].Color         = mColor;
	mCircleVertices[i].Thickness     = thickness;
	mCircleVertices[i].Smoothness    = smoothness;

	++mCircleCount;
	++mStats.QuadCount;
}
