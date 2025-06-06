#pragma once

#include <Color.hpp>
#include <Common.hpp>
#include <GPUBuffers.hpp>
#include <Texture.hpp>

class Window;
class WindowSettings;
using RenderDevicePtr = UniquePtr<class RenderDevice>;

enum class RenderAPI
{
	None,
	GL,
	GLES,
	VK,
	DX11,
	DX12,
	ML  // metal
};

struct RenderDeviceInfo
{
	String  Name;
	String  Vendor;
	Version DriverVersion;
	bool    ASTC;             // Adaptive scalable texture compression
	bool    S3TC;             // or DXT1 to DXT5
	bool    ETC1;             // Ericsson texture compression 1
	bool    ETC2;             // Ericsson texture compression 2
	bool    PVRTC;            // PowerVR texture compression
	bool    BC4BC5;           // BC4 and BC5
	bool    BC6HBC7;          // BC6H and BC7
	u32     NumTextureUnits;  // number of available texture samplers in fragment
	                          // shader
	u32 MaxTextureWidth;
	u32 MaxTextureHeight;
	u32 NumSamples;
};

enum class BlendFunc
{
	Zero,
	One,
	SrcColor,
	OneMinusSrcColor,
	DstColor,
	OneMinusDstColor,
	SrcAlpha,
	OneMinusSrcAlpha,
	DstAlpha,
	OneMinusDstAlpha,
	ConstantColor,
	OneMinusConstantColor,
	ConstantAlpha,
	OneMinusConstantAlpha
};

class RenderDevice
{
public:
	RenderDevice()                               = default;
	RenderDevice(const RenderDevice&)            = delete;
	RenderDevice& operator=(const RenderDevice&) = delete;
	virtual ~RenderDevice()                      = default;

	static RenderDevicePtr Create();

	static RenderAPI        GetAPI();
	const RenderDeviceInfo& GetInfo() const;

	virtual void SetClearColor(Color color) = 0;
	virtual void Clear()                    = 0;

	virtual void EnableBlending(bool enable)                             = 0;
	virtual void SetBlendFunc(BlendFunc src, BlendFunc dst, Color color) = 0;
	virtual bool IsBlendingEnable() const                                = 0;

	virtual void UpdateViewport(u32 x, u32 y, u32 width, u32 height) = 0;

	virtual void SetPointSize(float size) = 0;

	virtual void DrawIndexed(const VertexArrayPtr& va, u32 index_count) = 0;

private:
	static RenderAPI sAPI;

protected:
	RenderDeviceInfo mInfo {};
};
