#pragma once

#include <GPUBuffers.hpp>

class VertexBufferGL final: public VertexBuffer
{
public:
	VertexBufferGL(std::initializer_list<Vertex> layout, U32 count);
	~VertexBufferGL();

	const std::vector<Vertex>& GetLayout() const override;
	void SetLayout(std::initializer_list<Vertex> layout) override;
	U32  GetCount() const override;
	void SetData(const void* data, U32 count) override;
	U32  GetStride() const override;
	U32  GetID() const override;

private:
	U32                 mID {0};
	std::vector<Vertex> mLayout;
	U32                 mCount;
	U32                 mStride {0};
};

class IndexBufferGL final: public IndexBuffer
{
public:
	IndexBufferGL(const U32* data, U32 count);
	~IndexBufferGL();

	U32  GetCount() const override;
	void SetData(const U32* data, U32 count) override;
	U32  GetID() const override;

private:
	U32 mID {0};
	U32 mCount;
};

class VertexArrayGL final: public VertexArray
{
public:
	VertexArrayGL();
	~VertexArrayGL();

	void AttachIndexBuffer(const IndexBufferPtr& ib) override;
	void AttachVertexBuffer(const VertexBufferPtr& vb) override;

	void Bind() const override;
	void Unbind() const override;

private:
	U32 VertexTypeMap(VertexType type);

private:
	U32 mID {0};
};