#pragma once

#include <Common.hpp>

class VertexBuffer;
class IndexBuffer;
class VertexArray;

using VertexBufferPtr = SharedPtr<VertexBuffer>;
using IndexBufferPtr  = SharedPtr<IndexBuffer>;
using VertexArrayPtr  = SharedPtr<VertexArray>;


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

uword VertexTypeCount(VertexType type);
uword VertexTypeSize(VertexType type);

struct Vertex
{
	Vertex(VertexType type, bool normalize = false)
	        : Type(type), Offset(0), Normalize(normalize)
	{
	}

	VertexType Type;
	uword      Offset;
	bool       Normalize;
};

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;

	static VertexBufferPtr Create(std::initializer_list<Vertex> layout, uword count,
	                              uword stride);

	virtual const Vector<Vertex>& GetLayout() const               = 0;
	virtual void  SetLayout(std::initializer_list<Vertex> layout) = 0;
	virtual uword GetCount() const                                = 0;
	virtual void  SetData(const void* data, uword count)          = 0;
	virtual uword GetStride() const                               = 0;
	virtual uword GetID() const                                   = 0;
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() = default;

	static IndexBufferPtr Create(const uword* data, uword count);

	virtual uword GetCount() const                        = 0;
	virtual void  SetData(const uword* data, uword count) = 0;
	virtual uword GetID() const                           = 0;
};

class VertexArray
{
public:
	virtual ~VertexArray() = default;

	static VertexArrayPtr Create();

	virtual void AttachIndexBuffer(const IndexBufferPtr& ib)   = 0;
	virtual void AttachVertexBuffer(const VertexBufferPtr& vb) = 0;

	virtual uword           GetVertexBufferCount() const   = 0;
	virtual VertexBufferPtr GetVertexBuffer(uword i) const = 0;
	virtual IndexBufferPtr  GetIndexBuffer() const         = 0;

	virtual void Bind() const   = 0;
	virtual void Unbind() const = 0;
};
