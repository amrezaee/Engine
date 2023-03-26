#pragma once

#include <GPUBuffers.hpp>

class VertexBufferGL final: public VertexBuffer
{
public:
	VertexBufferGL(std::initializer_list<Vertex> layout, uword count, uword stride);
	~VertexBufferGL() override;

	const Vector<Vertex>& GetLayout() const override;
	void                  SetLayout(std::initializer_list<Vertex> layout) override;
	uword                 GetCount() const override;
	void                  SetData(const void* data, uword count) override;
	uword                 GetStride() const override;
	uword                 GetID() const override;

private:
	uword          mID {0};
	Vector<Vertex> mLayout;
	uword          mCount;
	uword          mStride;
};

class IndexBufferGL final: public IndexBuffer
{
public:
	IndexBufferGL(const uword* data, uword count);
	~IndexBufferGL() override;

	uword GetCount() const override;
	void  SetData(const uword* data, uword count) override;
	uword GetID() const override;

private:
	uword mID {0};
	uword mCount;
};

class VertexArrayGL final: public VertexArray
{
public:
	VertexArrayGL();
	~VertexArrayGL() override;

	void AttachIndexBuffer(const IndexBufferPtr& ib) override;
	void AttachVertexBuffer(const VertexBufferPtr& vb) override;

	uword           GetVertexBufferCount() const override;
	VertexBufferPtr GetVertexBuffer(uword i) const override;
	IndexBufferPtr  GetIndexBuffer() const override;

	void Bind() const override;
	void Unbind() const override;

private:
	static uword VertexTypeMap(VertexType type);

private:
	uword                   mID {0};
	IndexBufferPtr          mIB;
	Vector<VertexBufferPtr> mVBList;
	uword                   mVBIndex {0};
};
