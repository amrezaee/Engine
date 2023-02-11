#pragma once

#include <Common.hpp>

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

U32 VertexTypeCount(VertexType type);
U32 VertexTypeSize(VertexType type);

class VertexBuffer;
class IndexBuffer;
class VertexArray;

using VertexBufferPtr = Sptr<VertexBuffer>;
using IndexBufferPtr  = Sptr<IndexBuffer>;
using VertexArrayPtr  = Sptr<VertexArray>;

struct Vertex
{
	Vertex(VertexType atype, bool anormalize = false)
	        : type(atype), offset(0), normalize(anormalize)
	{
	}

	VertexType type;
	U32        offset;
	bool       normalize;
};

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;

	static VertexBufferPtr Create(std::initializer_list<Vertex> layout, U32 count);

	virtual const std::vector<Vertex>& GetLayout() const         = 0;
	virtual void SetLayout(std::initializer_list<Vertex> layout) = 0;
	virtual U32  GetCount() const                                = 0;
	virtual void SetData(const void* data, U32 count)            = 0;
	virtual U32  GetStride() const                               = 0;
	virtual U32  GetID() const                                   = 0;
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() = default;

	static IndexBufferPtr Create(const U32* data, U32 count);

	virtual U32  GetCount() const                    = 0;
	virtual void SetData(const U32* data, U32 count) = 0;
	virtual U32  GetID() const                       = 0;
};

class VertexArray
{
public:
	virtual ~VertexArray() = default;

	static VertexArrayPtr Create();

	virtual void AttachIndexBuffer(const IndexBufferPtr& ib)   = 0;
	virtual void AttachVertexBuffer(const VertexBufferPtr& vb) = 0;

	virtual void Bind() const   = 0;
	virtual void Unbind() const = 0;
};