#include <GPUBuffersGL.hpp>

#include <Assert.hpp>

#include <glad/gl.h>

VertexBufferGL::VertexBufferGL(std::initializer_list<Vertex> layout, U32 count)
        : mLayout(layout), mCount(count)
{
	U32 offset {0};
	U32 size {0};
	for(auto& e : mLayout)
	{
		e.offset = offset;
		size     = VertexTypeSize(e.type);
		offset += size;
		mStride += size;
	}
	glCreateBuffers(1, &mID);
	glNamedBufferStorage(mID, count * mStride, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

VertexBufferGL::~VertexBufferGL()
{
	glDeleteBuffers(1, &mID);
}

const std::vector<Vertex>& VertexBufferGL::GetLayout() const
{
	return mLayout;
}

void VertexBufferGL::SetLayout(std::initializer_list<Vertex> layout)
{
	mLayout = layout;
}

U32 VertexBufferGL::GetCount() const
{
	return mCount;
}

void VertexBufferGL::SetData(const void* data, U32 count)
{
	glNamedBufferSubData(mID, 0, count * mStride, data);
}

U32 VertexBufferGL::GetStride() const
{
	return mStride;
}

U32 VertexBufferGL::GetID() const
{
	return mID;
}


IndexBufferGL::IndexBufferGL(const U32* data, U32 count): mCount(count)
{
	glCreateBuffers(1, &mID);
	glNamedBufferStorage(mID, count * sizeof(U32), data, GL_DYNAMIC_STORAGE_BIT);
}

IndexBufferGL::~IndexBufferGL()
{
	glDeleteBuffers(1, &mID);
}

U32 IndexBufferGL::GetCount() const
{
	return mCount;
}

void IndexBufferGL::SetData(const U32* data, U32 count)
{
	glNamedBufferSubData(mID, 0, count * sizeof(U32), data);
}

U32 IndexBufferGL::GetID() const
{
	return mID;
}


VertexArrayGL::VertexArrayGL()
{
	glCreateVertexArrays(1, &mID);
}

VertexArrayGL::~VertexArrayGL()
{
	glDeleteVertexArrays(1, &mID);
}

void VertexArrayGL::AttachIndexBuffer(const IndexBufferPtr& ib)
{
	glVertexArrayElementBuffer(mID, ib->GetID());
}

void VertexArrayGL::AttachVertexBuffer(const VertexBufferPtr& vb)
{
	glVertexArrayVertexBuffer(mID, 0, vb->GetID(), 0, vb->GetStride());

	auto layout  = vb->GetLayout();
	U32  attribs = U32(layout.size());

	for(U32 i = 0; i < attribs; ++i)
	{
		glEnableVertexArrayAttrib(mID, i);
		glVertexArrayAttribFormat(mID, i, VertexTypeCount(layout[i].type),
		                          VertexTypeMap(layout[i].type),
		                          layout[i].normalize ? GL_TRUE : GL_FALSE,
		                          layout[i].offset);
		glVertexArrayAttribBinding(mID, i, 0);
	}
}

void VertexArrayGL::Bind() const
{
	glBindVertexArray(mID);
}

void VertexArrayGL::Unbind() const
{
	glBindVertexArray(0);
}

U32 VertexArrayGL::VertexTypeMap(VertexType type)
{
	switch(type)
	{
	case VertexType::Bool: return GL_BOOL;

	case VertexType::UInt4:
	case VertexType::UInt3:
	case VertexType::UInt2:
	case VertexType::UInt: return GL_UNSIGNED_INT;

	case VertexType::Int4:
	case VertexType::Int3:
	case VertexType::Int2:
	case VertexType::Int: return GL_INT;

	case VertexType::Float4:
	case VertexType::Float3:
	case VertexType::Float2:
	case VertexType::Float: return GL_FLOAT;

	case VertexType::UByte4:
	case VertexType::UByte3:
	case VertexType::UByte2:
	case VertexType::UByte: return GL_UNSIGNED_BYTE;

	case VertexType::Byte4:
	case VertexType::Byte3:
	case VertexType::Byte2:
	case VertexType::Byte: return GL_BYTE;

	default: return 0;
	}
}
