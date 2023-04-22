#include <GPUBuffersGL.hpp>

#include <Assert.hpp>
#include <Logger.hpp>

VertexBufferGL::VertexBufferGL(std::initializer_list<Vertex> layout, u32 count,
                               u32 stride)
        : mLayout(layout), mCount(count), mStride(stride)
{
	u32 offset {0};
	for(auto& e : mLayout)
	{
		e.Offset = offset;
		offset += VertexTypeSize(e.Type);
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

u32 VertexBufferGL::GetCount() const
{
	return mCount;
}

void VertexBufferGL::SetData(const void* data, u32 count)
{
	glNamedBufferSubData(mID, 0, count * mStride, data);
}

u32 VertexBufferGL::GetStride() const
{
	return mStride;
}

u32 VertexBufferGL::GetID() const
{
	return mID;
}


IndexBufferGL::IndexBufferGL(const u32* data, u32 count): mCount(count)
{
	glCreateBuffers(1, &mID);
	glNamedBufferStorage(mID, count * (GLsizeiptr)sizeof(u32), data,
	                     GL_DYNAMIC_STORAGE_BIT);
}

IndexBufferGL::~IndexBufferGL()
{
	glDeleteBuffers(1, &mID);
}

u32 IndexBufferGL::GetCount() const
{
	return mCount;
}

void IndexBufferGL::SetData(const u32* data, u32 count)
{
	glNamedBufferSubData(mID, 0, count * (GLsizeiptr)sizeof(u32), data);
}

u32 IndexBufferGL::GetID() const
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
	mIB = ib;
	glVertexArrayElementBuffer(mID, ib->GetID());
}

void VertexArrayGL::AttachVertexBuffer(const VertexBufferPtr& vb)
{
	glVertexArrayVertexBuffer(mID, mVBIndex, vb->GetID(), 0,
	                          (GLsizei)vb->GetStride());

	auto layout  = vb->GetLayout();
	auto attribs = u32(layout.size());

	for(u32 i = 0; i < attribs; ++i)
	{
		VertexType type   = layout[i].Type;
		u32        offset = layout[i].Offset;

		glEnableVertexArrayAttrib(mID, i);
		glVertexArrayAttribBinding(mID, i, mVBIndex);

		if(layout[i].Normalize)
		{
			glVertexArrayAttribFormat(mID, i, (GLint)VertexTypeCount(type),
			                          VertexTypeMap(type), GL_TRUE, offset);
			continue;
		}

		switch(type)
		{
		case VertexType::Float:
		case VertexType::Float2:
		case VertexType::Float3:
		case VertexType::Float4:
		{
			glVertexArrayAttribFormat(mID, i, (GLint)VertexTypeCount(type),
			                          VertexTypeMap(type), GL_FALSE, offset);
		}
		break;

		case VertexType::Byte:
		case VertexType::Byte2:
		case VertexType::Byte3:
		case VertexType::Byte4:
		case VertexType::UByte:
		case VertexType::UByte2:
		case VertexType::UByte3:
		case VertexType::UByte4:
		case VertexType::Bool:
		case VertexType::UInt:
		case VertexType::UInt2:
		case VertexType::UInt3:
		case VertexType::UInt4:
		case VertexType::Int:
		case VertexType::Int2:
		case VertexType::Int3:
		case VertexType::Int4:
		{
			glVertexArrayAttribIFormat(mID, i, (GLint)VertexTypeCount(type),
			                           VertexTypeMap(type), offset);
		}
		break;

		default: ERROR("Unknown vertex type");
		}
	}

	mVBList.push_back(vb);
	++mVBIndex;
}

u32 VertexArrayGL::GetVertexBufferCount() const
{
	return u32(mVBList.size());
}

VertexBufferPtr VertexArrayGL::GetVertexBuffer(u32 i) const
{
	if(!mVBList.empty())
		return mVBList[mVBList.size() - 1 - i];
	else
	{
		WARN("No vertex buffer attached");
		return nullptr;
	}
}

IndexBufferPtr VertexArrayGL::GetIndexBuffer() const
{
	if(mIB)
		return mIB;
	else
	{
		WARN("No index buffer attached");
		return nullptr;
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

u32 VertexArrayGL::VertexTypeMap(VertexType type)
{
	switch(type)
	{
	case VertexType::Bool: return GL_BOOL;

	case VertexType::UInt:
	case VertexType::UInt2:
	case VertexType::UInt3:
	case VertexType::UInt4: return GL_UNSIGNED_INT;

	case VertexType::Int:
	case VertexType::Int2:
	case VertexType::Int3:
	case VertexType::Int4: return GL_INT;

	case VertexType::Float:
	case VertexType::Float2:
	case VertexType::Float3:
	case VertexType::Float4: return GL_FLOAT;

	case VertexType::UByte:
	case VertexType::UByte2:
	case VertexType::UByte3:
	case VertexType::UByte4: return GL_UNSIGNED_BYTE;

	case VertexType::Byte:
	case VertexType::Byte2:
	case VertexType::Byte3:
	case VertexType::Byte4: return GL_BYTE;

	default: return 0;
	}
}
