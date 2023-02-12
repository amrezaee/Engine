#include <GPUBuffers.hpp>

#include <Assert.hpp>
#include <GPUBuffersGL.hpp>
#include <RenderDevice.hpp>

VertexBufferPtr VertexBuffer::Create(std::initializer_list<Vertex> layout, U32 count,
                                     U32 stride)
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeSptr<VertexBufferGL>(layout, count, stride);
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}

IndexBufferPtr IndexBuffer::Create(const U32* data, U32 count)
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeSptr<IndexBufferGL>(data, count);
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}

VertexArrayPtr VertexArray::Create()
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeSptr<VertexArrayGL>();
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}

U32 VertexTypeSize(VertexType type)
{
	switch(type)
	{
	case VertexType::Byte:
	case VertexType::UByte:
	case VertexType::Bool: return 1;

	case VertexType::UByte2:
	case VertexType::Byte2: return 2;

	case VertexType::UByte3:
	case VertexType::Byte3: return 3;

	case VertexType::UByte4:
	case VertexType::Byte4: return 4;

	case VertexType::Int:
	case VertexType::UInt:
	case VertexType::Float: return 4;

	case VertexType::Int2:
	case VertexType::UInt2:
	case VertexType::Float2: return 8;

	case VertexType::Int3:
	case VertexType::UInt3:
	case VertexType::Float3: return 12;

	case VertexType::Int4:
	case VertexType::UInt4:
	case VertexType::Float4: return 16;

	default: return 0;
	}
}

U32 VertexTypeCount(VertexType type)
{
	switch(type)
	{
	case VertexType::Bool:
	case VertexType::Int:
	case VertexType::UInt:
	case VertexType::Byte:
	case VertexType::UByte:
	case VertexType::Float: return 1;

	case VertexType::Int2:
	case VertexType::UInt2:
	case VertexType::Byte2:
	case VertexType::UByte2:
	case VertexType::Float2: return 2;

	case VertexType::Int3:
	case VertexType::UInt3:
	case VertexType::Byte3:
	case VertexType::UByte3:
	case VertexType::Float3: return 3;

	case VertexType::Int4:
	case VertexType::UInt4:
	case VertexType::Byte4:
	case VertexType::UByte4:
	case VertexType::Float4: return 4;

	default: return 0;
	}
}
