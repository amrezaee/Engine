#include <WindowGLFW.hpp>

#include <Logger.hpp>

uword WindowGLFW::sWindowCount = 0;

void CloseCallback(GLFWwindow* win)
{
	auto w = (WindowGLFW*)glfwGetWindowUserPointer(win);
	w->CloseSignal();
}

void SizeCallback(GLFWwindow* win, int width, int height)
{
	auto w = (WindowGLFW*)glfwGetWindowUserPointer(win);
	w->SizeSignal(Vec2ui {width, height});
}

void FramebufferCallback(GLFWwindow* win, int width, int height)
{
	auto w = (WindowGLFW*)glfwGetWindowUserPointer(win);
	w->FramebufferSignal(Vec2ui {width, height});

	if(w->GetWindowMode() == WindowMode::Windowed)
	{
		w->mSettings.Resolution.x = (uword)width;
		w->mSettings.Resolution.y = (uword)height;
	}
}

void PositionCallback(GLFWwindow* win, int x, int y)
{
	auto w = (WindowGLFW*)glfwGetWindowUserPointer(win);
	w->PositionSignal(Vec2ui {x, y});

	if(w->GetWindowMode() == WindowMode::Windowed)
	{
		w->mSettings.Position.x = (uword)x;
		w->mSettings.Position.y = (uword)y;
	}
}

void FocusCallback(GLFWwindow* win, int state)
{
	auto w = (WindowGLFW*)glfwGetWindowUserPointer(win);

	w->FocusSignal(state == GLFW_TRUE);
	w->mSettings.Focused = state == GLFW_TRUE;
}

void KeyCallback(GLFWwindow* win, int key, int, int action, int)
{
	auto w = (WindowGLFW*)glfwGetWindowUserPointer(win);

	if(action == GLFW_PRESS)
		w->KeySignal((Key)key, Key::None);
	else if(action == GLFW_RELEASE)
		w->KeySignal(Key::None, (Key)key);
}

void MouseCallback(GLFWwindow* win, int button, int action, int)
{
	auto w = (WindowGLFW*)glfwGetWindowUserPointer(win);

	if(action == GLFW_PRESS)
		w->MouseSignal((MouseButton)button, MouseButton::None);
	else
		w->MouseSignal(MouseButton::None, (MouseButton)button);
}

void CursorCallback(GLFWwindow* win, double xpos, double ypos)
{
	auto w = (WindowGLFW*)glfwGetWindowUserPointer(win);
	w->CursorSignal(Vec2 {xpos, ypos});
}

void ScrollCallback(GLFWwindow* win, double xoffset, double yoffset)
{
	auto w = (WindowGLFW*)glfwGetWindowUserPointer(win);
	w->ScrollSignal(Vec2 {xoffset, yoffset});
}

static void ErrorCallback(int code, const char* desc)
{
	ERROR("#{}: {}", code, desc);
}

WindowGLFW::WindowGLFW(const WindowSettings& settings)
        : Window(settings), mWindow(nullptr)
{
	glfwSetErrorCallback(ErrorCallback);

	if(sWindowCount == 0)
		glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#ifdef DEBUG_BUILD
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

	glfwWindowHint(GLFW_STENCIL_BITS, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 0);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, mSettings.Resizable);
	glfwWindowHint(GLFW_DECORATED, !mSettings.Borderless);
	glfwWindowHint(GLFW_FOCUSED, mSettings.Focused);

	mWindow = glfwCreateWindow((int)mSettings.Resolution.x,
	                           (int)mSettings.Resolution.y, mSettings.Title.c_str(),
	                           nullptr, nullptr);

	glfwMakeContextCurrent(mWindow);

	glfwSetWindowUserPointer(mWindow, this);
	glfwSetFramebufferSizeCallback(mWindow, FramebufferCallback);
	glfwSetWindowCloseCallback(mWindow, CloseCallback);
	glfwSetWindowSizeCallback(mWindow, SizeCallback);
	glfwSetWindowFocusCallback(mWindow, FocusCallback);
	glfwSetWindowPosCallback(mWindow, PositionCallback);
	glfwSetKeyCallback(mWindow, KeyCallback);
	glfwSetMouseButtonCallback(mWindow, MouseCallback);
	glfwSetCursorPosCallback(mWindow, CursorCallback);
	glfwSetScrollCallback(mWindow, ScrollCallback);

	SetHidden(true);
	SetWindowMode(mSettings.Mode);
	SetPosition(mSettings.Position);
	SetHidden(false);

	SetVSyncMode(mSettings.VSync);
	SetHidden(mSettings.Hidden);

	++sWindowCount;
}

WindowGLFW::~WindowGLFW()
{
	if(!mWindow)
	{
		WARN("Window already destroyed");
		return;
	}

	glfwDestroyWindow(mWindow);
	mWindow = nullptr;
	--sWindowCount;

	if(sWindowCount == 0)
		glfwTerminate();
}

const String& WindowGLFW::GetTitle() const
{
	return mSettings.Title;
}

void WindowGLFW::SetTitle(const String& title)
{
	mSettings.Title = title;
	glfwSetWindowTitle(mWindow, title.c_str());
}

uword WindowGLFW::GetWidth() const
{
	return mSettings.Resolution.x;
}

uword WindowGLFW::GetHeight() const
{
	return mSettings.Resolution.y;
}

Vec2ui WindowGLFW::GetResolution() const
{
	return mSettings.Resolution;
}

void WindowGLFW::SetWidth(uword width)
{
	mSettings.Resolution.x = width;
	glfwSetWindowSize(mWindow, int(width), int(mSettings.Resolution.y));
}

void WindowGLFW::SetHeight(uword height)
{
	mSettings.Resolution.y = height;
	glfwSetWindowSize(mWindow, int(mSettings.Resolution.x), int(height));
}

void WindowGLFW::SetResolution(Vec2ui resolution)
{
	mSettings.Resolution = resolution;
	glfwSetWindowSize(mWindow, int(resolution.x), int(resolution.y));
}

Vec2ui WindowGLFW::GetPosition() const
{
	return mSettings.Position;
}

void WindowGLFW::SetPosition(Vec2ui pos)
{
	mSettings.Position = pos;
	glfwSetWindowPos(mWindow, int(pos.x), int(pos.y));
}

WindowMode WindowGLFW::GetWindowMode() const
{
	return mSettings.Mode;
}

bool WindowGLFW::IsFullscreen() const
{
	return mSettings.Mode != WindowMode::Windowed;
}

void WindowGLFW::SetWindowMode(WindowMode mode)
{
	if(mode == mSettings.Mode)
		return;

	mSettings.Mode       = mode;
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	switch(mode)
	{
	case WindowMode::Windowed:
	{
		glfwSetWindowMonitor(mWindow, nullptr, (int)mSettings.Position.x,
		                     (int)mSettings.Position.y, (int)mSettings.Resolution.x,
		                     (int)mSettings.Resolution.y, GLFW_DONT_CARE);
	}
	break;

	case WindowMode::BorderlessFS:
	{
		const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(mWindow, monitor, 0, 0, vidmode->width, vidmode->height,
		                     vidmode->refreshRate);
	}
	break;

	case WindowMode::ExclusiveFS:
	{
		glfwSetWindowMonitor(mWindow, monitor, 0, 0, (int)mSettings.Resolution.x,
		                     (int)mSettings.Resolution.y, GLFW_DONT_CARE);
	}
	break;
	}
}

VSyncMode WindowGLFW::GetVSyncMode() const
{
	return mSettings.VSync;
}

void WindowGLFW::SetVSyncMode(VSyncMode mode)
{
	mSettings.VSync = mode;
	glfwSwapInterval(int(mode));
}

uword WindowGLFW::GetMSAA() const
{
	return mSettings.MSAA;
}

void WindowGLFW::SetMSAA(uword samples)
{
	if(samples != 0 && samples != 2 && samples != 4 && samples != 8 &&
	   samples != 16 && samples != 32)
		return;

	mSettings.MSAA = samples;

	// TODO: implement
}

bool WindowGLFW::IsSRGB() const
{
	return mSettings.SRGB;
}
void WindowGLFW::SetSRGB(bool srgb)
{
	(void)srgb;
}

bool WindowGLFW::IsResizable() const
{
	return mSettings.Resizable;
}

void WindowGLFW::SetResizable(bool resizable)
{
	mSettings.Resizable = resizable;
	glfwSetWindowAttrib(mWindow, GLFW_RESIZABLE, resizable);
}

bool WindowGLFW::IsBorderless() const
{
	return mSettings.Borderless;
}

void WindowGLFW::SetBorderless(bool borderless)
{
	mSettings.Borderless = borderless;
	glfwSetWindowAttrib(mWindow, GLFW_DECORATED, !borderless);
}

bool WindowGLFW::IsFocused() const
{
	return mSettings.Focused;
}

void WindowGLFW::SetFocus(bool focused)
{
	mSettings.Focused = focused;
	if(focused)
		glfwFocusWindow(mWindow);
}

bool WindowGLFW::IsHidden() const
{
	return mSettings.Hidden;
}

void WindowGLFW::SetHidden(bool hidden)
{
	mSettings.Hidden = hidden;

	if(hidden)
		glfwHideWindow(mWindow);
	else
		glfwShowWindow(mWindow);
}

void* WindowGLFW::GetHandle() const
{
	return mWindow;
}

void WindowGLFW::SwapBuffers()
{
	glfwSwapBuffers(mWindow);
}

void WindowGLFW::PollEvents()
{
	glfwPollEvents();
}
