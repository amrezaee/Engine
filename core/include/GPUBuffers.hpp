#pragma once

#include <Common.hpp>

using VertexBufferPtr = SharedPtr<class VertexBuffer>;
using IndexBufferPtr  = SharedPtr<class IndexBuffer>;
using VertexArrayPtr  = SharedPtr<class VertexArray>;

enum class VertexType
{
	Byte,
	Byte2,
	Byte3,
	Byte4,
	UByte,
	UByte2,
	UByte3,
	UByte4,
	Float,
	Float2,
	Float3,
	Float4,
	Int,
	Int2,
	Int3,
	Int4,
	UInt,
	UInt2,
	UInt3,
	UInt4,
	Bool,
};

u32 VertexTypeCount(VertexType type);
u32 VertexTypeSize(VertexType type);

struct Vertex
{
	explicit Vertex(VertexType type, bool normalize = false)
	        : Type(type),
	          Offset(0),
	          Normalize(normalize)
	{
	}

	VertexType Type;
	u32        Offset;
	bool       Normalize;
};

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;

	static VertexBufferPtr Create(std::initializer_list<Vertex> layout,
	                              u32                           count,
	                              u32                           stride);

	virtual const Vector<Vertex>& GetLayout() const              = 0;
	virtual void SetLayout(std::initializer_list<Vertex> layout) = 0;
	virtual u32  GetCount() const                                = 0;
	virtual void SetData(const void* data, u32 count)            = 0;
	virtual u32  GetStride() const                               = 0;
	virtual u32  GetID() const                                   = 0;
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() = default;

	static IndexBufferPtr Create(const u32* data, u32 count);

	virtual u32  GetCount() const                    = 0;
	virtual void SetData(const u32* data, u32 count) = 0;
	virtual u32  GetID() const                       = 0;
};

class VertexArray
{
public:
	virtual ~VertexArray() = default;

	static VertexArrayPtr Create();

	virtual void AttachIndexBuffer(const IndexBufferPtr& ib)   = 0;
	virtual void AttachVertexBuffer(const VertexBufferPtr& vb) = 0;

	virtual u32             GetVertexBufferCount() const = 0;
	virtual VertexBufferPtr GetVertexBuffer(u32 i) const = 0;
	virtual IndexBufferPtr  GetIndexBuffer() const       = 0;

	virtual void Bind() const   = 0;
	virtual void Unbind() const = 0;
};
