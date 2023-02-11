#pragma once

#include <RenderDevice.hpp>

class RenderDeviceGL: public RenderDevice
{
public:
	RenderDeviceGL()  = default;
	~RenderDeviceGL() = default;
	PREVENT_COPY(RenderDeviceGL);

	void Initialize() override;

	void SetClearColor(Color color = Color::BLACK) override;
	void Clear() override;

	void EnableBlending(bool enable) override;
	void SetBlendFunc(BlendFunc src, BlendFunc dst,
	                  Color color = Color::WHITE) override;
	bool IsBlendingEnable() const override;

	void UpdateViewport(U32 x, U32 y, U32 width, U32 height) override;

	void SetPointSize(float size) override;

	void DrawIndexed(const VertexArrayPtr& va, U32 index_count) override;
	void DrawLine(const VertexArrayPtr& va, U32 vertex_count) override;

private:
	I32 BlendFuncMap(BlendFunc func);
};