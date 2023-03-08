#pragma once

#include <RenderDevice.hpp>

class RenderDeviceGL: public RenderDevice
{
public:
	void Initialize() override;

	void SetClearColor(Color color = Color::BLACK) override;
	void Clear() override;

	void EnableBlending(bool enable) override;
	void SetBlendFunc(BlendFunc src, BlendFunc dst,
	                  Color color = Color::WHITE) override;
	bool IsBlendingEnable() const override;

	void UpdateViewport(uword x, uword y, uword width, uword height) override;

	void SetPointSize(float size) override;

	void DrawIndexed(const VertexArrayPtr& va, uword index_count = 0) override;

private:
	word BlendFuncMap(BlendFunc func);
};
