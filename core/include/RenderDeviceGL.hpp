#pragma once

#include <RenderDevice.hpp>

class RenderDeviceGL: public RenderDevice
{
public:
	RenderDeviceGL();

	void SetClearColor(Color color) override;
	void Clear() override;

	void EnableBlending(bool enable) override;
	void SetBlendFunc(BlendFunc src, BlendFunc dst, Color color) override;
	bool IsBlendingEnable() const override;

	void UpdateViewport(u32 x, u32 y, u32 width, u32 height) override;

	void SetPointSize(float size) override;

	void DrawIndexed(const VertexArrayPtr& va, u32 index_count) override;

private:
	static i32 BlendFuncMap(BlendFunc func);
};
