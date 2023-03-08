#include <WindowGLFW.hpp>

#include <Assert.hpp>
#include <Logger.hpp>
#include <Renderer.hpp>

static uword window_count = 0;

void CloseCallback(GLFWwindow* win)
{
	((WindowGLFW*)glfwGetWindowUserPointer(win))->CloseSignal();
}

void FramebufferCallback(GLFWwindow* win, int width, int height)
{
	WindowGLFW* w = (WindowGLFW*)glfwGetWindowUserPointer(win);

	w->FramebufferSignal({width, height});
	w->mSettings.Resolution.x = width;
	w->mSettings.Resolution.y = height;
}

void FocusCallback(GLFWwindow* win, int state)
{
	WindowGLFW* w = (WindowGLFW*)glfwGetWindowUserPointer(win);

	w->FocusSignal(state == GLFW_TRUE ? true : false);
	w->mSettings.Focused = GLFW_TRUE ? true : false;
}

void SizeCallback(GLFWwindow* win, int width, int height)
{
	((WindowGLFW*)glfwGetWindowUserPointer(win))->SizeSignal({width, height});
}

void PositionCallback(GLFWwindow* win, int x, int y)
{
	WindowGLFW* w = (WindowGLFW*)glfwGetWindowUserPointer(win);
	w->PositionSignal({x, y});
	w->mSettings.Position.x = x;
	w->mSettings.Position.y = y;
}

static void ErrorCallback(int code, const char* desc)
{
	ERROR("#{}: {}", code, desc);
}

WindowGLFW::WindowGLFW(const WindowSettings& settings)
        : Window(settings), mHWin(nullptr)
{
	Create();
}

WindowGLFW::~WindowGLFW()
{
	Destroy();
}

void WindowGLFW::Create()
{
	glfwSetErrorCallback(ErrorCallback);

	if(window_count == 0)
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
	glfwWindowHint(GLFW_RESIZABLE, mSettings.Resizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, mSettings.Borderless ? GLFW_FALSE : GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, mSettings.Focused ? GLFW_TRUE : GLFW_FALSE);

	mHWin = glfwCreateWindow(int(mSettings.Resolution.x),
	                         int(mSettings.Resolution.y), mSettings.Title.c_str(),
	                         nullptr, nullptr);

	glfwMakeContextCurrent(mHWin);

	glfwSetWindowUserPointer(mHWin, this);
	glfwSetFramebufferSizeCallback(mHWin, FramebufferCallback);
	glfwSetWindowCloseCallback(mHWin, CloseCallback);
	glfwSetWindowSizeCallback(mHWin, SizeCallback);
	glfwSetWindowFocusCallback(mHWin, FocusCallback);
	glfwSetWindowPosCallback(mHWin, PositionCallback);

	SetHidden(true);
	SetWindowMode(mSettings.Mode);
	SetPosition(mSettings.Position);
	SetHidden(false);

	SetVSyncMode(mSettings.VSync);
	SetHidden(mSettings.Hidden);

	++window_count;
}

void WindowGLFW::Destroy()
{
	if(!mHWin)
	{
		WARN("Window already destroyed");
		return;
	}

	glfwDestroyWindow(mHWin);
	mHWin = nullptr;
	--window_count;

	if(window_count == 0)
		glfwTerminate();
}

const String& WindowGLFW::GetTitle() const
{
	return mSettings.Title;
}

void WindowGLFW::SetTitle(const String& title)
{
	mSettings.Title = title;
	glfwSetWindowTitle(mHWin, title.c_str());
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
	glfwSetWindowSize(mHWin, int(width), int(mSettings.Resolution.y));
}

void WindowGLFW::SetHeight(uword height)
{
	mSettings.Resolution.y = height;
	glfwSetWindowSize(mHWin, int(mSettings.Resolution.x), int(height));
}

void WindowGLFW::SetResolution(Vec2ui resolution)
{
	mSettings.Resolution = resolution;
	glfwSetWindowSize(mHWin, int(resolution.x), int(resolution.y));
}

Vec2ui WindowGLFW::GetPosition() const
{
	return mSettings.Position;
}

void WindowGLFW::SetPosition(Vec2ui pos)
{
	mSettings.Position = pos;
	glfwSetWindowPos(mHWin, int(pos.x), int(pos.y));
}

WindowMode WindowGLFW::GetWindowMode() const
{
	return mSettings.Mode;
}

bool WindowGLFW::IsFullscreen() const
{
	return mSettings.Mode != WindowMode::Windowed ? true : false;
}

void WindowGLFW::SetWindowMode(WindowMode mode)
{
	mSettings.Mode = mode;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	switch(mode)
	{
	case WindowMode::Windowed:
		glfwSetWindowMonitor(mHWin, nullptr, int(mSettings.Position.x),
		                     int(mSettings.Position.y), int(mSettings.Resolution.x),
		                     int(mSettings.Resolution.y), GLFW_DONT_CARE);
		break;

	case WindowMode::ExclusiveFS:
		glfwSetWindowMonitor(mHWin, monitor, int(mSettings.Position.x),
		                     int(mSettings.Position.y), int(mSettings.Resolution.x),
		                     int(mSettings.Resolution.y), GLFW_DONT_CARE);
		break;

	case WindowMode::BorderlessFS:
	{
		const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

		glfwSetWindowMonitor(mHWin, monitor, 0, 0, vidmode->width, vidmode->height,
		                     vidmode->refreshRate);
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
	if(samples != 0 || samples != 2 || samples != 4 || samples != 8 ||
	   samples != 16 || samples != 32)
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
	glfwSetWindowAttrib(mHWin, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
}

bool WindowGLFW::IsBorderless() const
{
	return mSettings.Borderless;
}

void WindowGLFW::SetBorderless(bool borderless)
{
	mSettings.Borderless = borderless;
	glfwSetWindowAttrib(mHWin, GLFW_DECORATED, borderless ? GLFW_FALSE : GLFW_TRUE);
}

bool WindowGLFW::IsFocused() const
{
	return mSettings.Focused;
}

void WindowGLFW::SetFocus(bool focused)
{
	mSettings.Focused = focused;
	if(focused)
		glfwFocusWindow(mHWin);
}

bool WindowGLFW::IsHidden() const
{
	return mSettings.Hidden;
}

void WindowGLFW::SetHidden(bool hidden)
{
	mSettings.Hidden = hidden;

	if(hidden)
		glfwHideWindow(mHWin);
	else
		glfwShowWindow(mHWin);
}

void* WindowGLFW::GetHandle() const
{
	return mHWin;
}

void WindowGLFW::SwapBuffers()
{
	glfwSwapBuffers(mHWin);
}

void WindowGLFW::PollEvents()
{
	glfwPollEvents();
}
