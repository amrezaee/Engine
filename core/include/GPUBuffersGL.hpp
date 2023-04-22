#pragma once

#include <GPUBuffers.hpp>

class VertexBufferGL final: public VertexBuffer
{
public:
	VertexBufferGL(std::initializer_list<Vertex> layout, u32 count, u32 stride);
	~VertexBufferGL() override;

	const Vector<Vertex>& GetLayout() const override;
	void                  SetLayout(std::initializer_list<Vertex> layout) override;
	u32                   GetCount() const override;
	void                  SetData(const void* data, u32 count) override;
	u32                   GetStride() const override;
	u32                   GetID() const override;

private:
	u32            mID {0};
	Vector<Vertex> mLayout;
	u32            mCount;
	u32            mStride;
};

class IndexBufferGL final: public IndexBuffer
{
public:
	IndexBufferGL(const u32* data, u32 count);
	~IndexBufferGL() override;

	u32  GetCount() const override;
	void SetData(const u32* data, u32 count) override;
	u32  GetID() const override;

private:
	u32 mID {0};
	u32 mCount;
};

class VertexArrayGL final: public VertexArray
{
public:
	VertexArrayGL();
	~VertexArrayGL() override;

	void AttachIndexBuffer(const IndexBufferPtr& ib) override;
	void AttachVertexBuffer(const VertexBufferPtr& vb) override;

	u32             GetVertexBufferCount() const override;
	VertexBufferPtr GetVertexBuffer(u32 i) const override;
	IndexBufferPtr  GetIndexBuffer() const override;

	void Bind() const override;
	void Unbind() const override;

private:
	static u32 VertexTypeMap(VertexType type);

private:
	u32                     mID {0};
	IndexBufferPtr          mIB;
	Vector<VertexBufferPtr> mVBList;
	u32                     mVBIndex {0};
};
