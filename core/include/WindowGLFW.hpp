#pragma once

#include <Window.hpp>

class WindowGLFW final: public Window
{
public:
	explicit WindowGLFW(const WindowSettings& settings);
	~WindowGLFW() override;

	const String& GetTitle() const override;
	void          SetTitle(const String& title) override;

	uword  GetWidth() const override;
	uword  GetHeight() const override;
	Vec2ui GetResolution() const override;
	void   SetWidth(uword width) override;
	void   SetHeight(uword height) override;
	void   SetResolution(Vec2ui resolution) override;

	Vec2ui GetPosition() const override;
	void   SetPosition(Vec2ui pos) override;

	WindowMode GetWindowMode() const override;
	bool       IsFullscreen() const override;
	void       SetWindowMode(WindowMode mode) override;

	VSyncMode GetVSyncMode() const override;
	void      SetVSyncMode(VSyncMode mode) override;

	uword GetMSAA() const override;
	void  SetMSAA(uword samples) override;

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
	friend void KeyCallback(GLFWwindow* win, int key, int scancode, int action,
	                        int mods);
	friend void MouseCallback(GLFWwindow* win, int button, int action, int mods);
	friend void CursorCallback(GLFWwindow* win, double xpos, double ypos);
	friend void ScrollCallback(GLFWwindow* win, double xoffset, double yoffset);

private:
	GLFWwindow* mHWin;
};

void CloseCallback(GLFWwindow* win);
void FramebufferCallback(GLFWwindow* win, int width, int height);
void FocusCallback(GLFWwindow* win, int state);
void SizeCallback(GLFWwindow* win, int width, int height);
void PositionCallback(GLFWwindow* win, int x, int y);
void KeyCallback(GLFWwindow* win, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow* win, int button, int action, int mods);
void CursorCallback(GLFWwindow* win, double xpos, double ypos);
void ScrollCallback(GLFWwindow* win, double xoffset, double yoffset);
