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

	void UpdateViewport(uword x, uword y, uword width, uword height) override;

	void SetPointSize(float size) override;

	void DrawIndexed(const VertexArrayPtr& va, uword index_count) override;

private:
	static word BlendFuncMap(BlendFunc func);
};
