#pragma once

#include <Window.hpp>

#include <GLFW/glfw3.h>

class WindowGLFW final: public Window
{
public:
	WindowGLFW(const WindowSettings& settings);
	PREVENT_COPY(WindowGLFW);
	~WindowGLFW();

public:
	void Create();
	void Destroy() override;

	const String& GetTitle() const;
	void          SetTitle(const String& title) override;

	U32    GetWidth() const override;
	U32    GetHeight() const override;
	Vec2ui GetResolution() const override;
	void   SetWidth(U32 width) override;
	void   SetHeight(U32 height) override;
	void   SetResolution(Vec2ui resolution) override;

	Vec2ui GetPosition() const override;
	void   SetPosition(Vec2ui pos) override;

	WindowMode GetWindowMode() const override;
	bool       IsFullscreen() const override;
	void       SetWindowMode(WindowMode mode) override;

	VSyncMode GetVSyncMode() const override;
	void      SetVSyncMode(VSyncMode mode) override;

	U32  GetMSAA() const override;
	void SetMSAA(U32 samples) override;

	bool IsSRGB() const override;
	void SetSRGB(bool srgb) override;

	bool IsResizable() const override;
	void SetResizable(bool resizable) override;

	bool IsBorderless() const override;
	void SetBorderless(bool borderless) override;

	bool IsFocused() const override;
	void SetFocus(bool focused) override;

	bool IsHidden() const override;
	void SetHidden(bool hidden) override;

	void* GetHandle() const override;

	void SwapBuffers() override;
	void PollEvents() override;

	friend void CloseCallback(GLFWwindow* win);
	friend void FramebufferCallback(GLFWwindow* win, int width, int height);
	friend void FocusCallback(GLFWwindow* win, int state);
	friend void SizeCallback(GLFWwindow* win, int width, int height);
	friend void PositionCallback(GLFWwindow* win, int x, int y);

private:
	GLFWwindow* mHWin;
};

void CloseCallback(GLFWwindow* win);
void FramebufferCallback(GLFWwindow* win, int width, int height);
void FocusCallback(GLFWwindow* win, int state);
void SizeCallback(GLFWwindow* win, int width, int height);
void PositionCallback(GLFWwindow* win, int x, int y);