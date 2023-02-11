#include <WindowGLFW.hpp>

#include <Assert.hpp>
#include <Logger.hpp>
#include <Renderer.hpp>

static U32 window_count = 0;

static void CloseCallback(GLFWwindow* win)
{
	((WindowGLFW*)glfwGetWindowUserPointer(win))->CloseSignal();
}

static void FramebufferCallback(GLFWwindow* win, int width, int height)
{
	WindowGLFW* w = (WindowGLFW*)glfwGetWindowUserPointer(win);

	w->FramebufferSignal({U32(width), U32(height)});
	w->SetResolution({U32(width), U32(height)});
}

static void FocusCallback(GLFWwindow* win, int state)
{
	WindowGLFW* w = (WindowGLFW*)glfwGetWindowUserPointer(win);

	w->FocusSignal(state == GLFW_TRUE ? true : false);
	w->SetFocus(state == GLFW_TRUE ? true : false);
}

static void SizeCallback(GLFWwindow* win, int width, int height)
{
	((WindowGLFW*)glfwGetWindowUserPointer(win))
	        ->SizeSignal({U32(width), U32(height)});
}

static void PositionCallback(GLFWwindow* win, int x, int y)
{
	WindowGLFW* w = (WindowGLFW*)glfwGetWindowUserPointer(win);
	w->PositionSignal({U32(x), U32(y)});
	w->SetPosition({U32(x), U32(y)});
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
	glfwWindowHint(GLFW_RESIZABLE, mSettings.mResizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, mSettings.mBorderless ? GLFW_FALSE : GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, mSettings.mFocused ? GLFW_TRUE : GLFW_FALSE);

	mHWin = glfwCreateWindow(int(mSettings.mResolution.x),
	                         int(mSettings.mResolution.y), mSettings.mTitle.c_str(),
	                         nullptr, nullptr);

	glfwMakeContextCurrent(mHWin);

	glfwSetWindowUserPointer(mHWin, this);
	glfwSetFramebufferSizeCallback(mHWin, FramebufferCallback);
	glfwSetWindowCloseCallback(mHWin, CloseCallback);
	glfwSetWindowSizeCallback(mHWin, SizeCallback);
	glfwSetWindowFocusCallback(mHWin, FocusCallback);
	glfwSetWindowPosCallback(mHWin, PositionCallback);

	glfwHideWindow(mHWin);
	SetWindowMode(mSettings.mWindowMode);
	glfwShowWindow(mHWin);

	SetVSyncMode(mSettings.mVSyncMode);
	SetHidden(mSettings.mHidden);

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
	return mSettings.mTitle;
}

void WindowGLFW::SetTitle(const String& title)
{
	mSettings.mTitle = title;
	glfwSetWindowTitle(mHWin, title.c_str());
}

U32 WindowGLFW::GetWidth() const
{
	return mSettings.mResolution.x;
}

U32 WindowGLFW::GetHeight() const
{
	return mSettings.mResolution.y;
}

Vec2ui WindowGLFW::GetResolution() const
{
	return mSettings.mResolution;
}

void WindowGLFW::SetWidth(U32 width)
{
	mSettings.mResolution.x = width;
	glfwSetWindowSize(mHWin, int(width), int(mSettings.mResolution.y));
}

void WindowGLFW::SetHeight(U32 height)
{
	mSettings.mResolution.y = height;
	glfwSetWindowSize(mHWin, int(mSettings.mResolution.x), int(height));
}

void WindowGLFW::SetResolution(Vec2ui resolution)
{
	mSettings.mResolution = resolution;
	glfwSetWindowSize(mHWin, int(resolution.x), int(resolution.y));
}

Vec2ui WindowGLFW::GetPosition() const
{
	return mSettings.mPosition;
}

void WindowGLFW::SetPosition(Vec2ui pos)
{
	mSettings.mPosition = pos;
	glfwSetWindowPos(mHWin, int(pos.x), int(pos.y));
}

WindowMode WindowGLFW::GetWindowMode() const
{
	return mSettings.mWindowMode;
}

bool WindowGLFW::IsFullscreen() const
{
	return mSettings.mWindowMode != WindowMode::Windowed ? true : false;
}

void WindowGLFW::SetWindowMode(WindowMode mode)
{
	if(mode == mSettings.mWindowMode)
		return;

	mSettings.mWindowMode = mode;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	switch(mode)
	{
	case WindowMode::Windowed:
		glfwSetWindowMonitor(mHWin, nullptr, int(mSettings.mPosition.x),
		                     int(mSettings.mPosition.y),
		                     int(mSettings.mResolution.x),
		                     int(mSettings.mResolution.y), GLFW_DONT_CARE);
		break;

	case WindowMode::ExclusiveFS:
		glfwSetWindowMonitor(mHWin, monitor, int(mSettings.mPosition.x),
		                     int(mSettings.mPosition.y),
		                     int(mSettings.mResolution.x),
		                     int(mSettings.mResolution.y), GLFW_DONT_CARE);
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
	return mSettings.mVSyncMode;
}

void WindowGLFW::SetVSyncMode(VSyncMode mode)
{
	mSettings.mVSyncMode = mode;
	glfwSwapInterval(int(mode));
}

U32 WindowGLFW::GetMSAA() const
{
	return mSettings.mMSAA;
}

void WindowGLFW::SetMSAA(U32 samples)
{
	if(samples != 0 || samples != 2 || samples != 4 || samples != 8 ||
	   samples != 16 || samples != 32)
		return;

	mSettings.mMSAA = samples;

	// implement
}

bool WindowGLFW::IsSRGB() const
{
	return mSettings.mSRGB;
}
void WindowGLFW::SetSRGB(bool srgb)
{
	UNUSED(srgb);
}

bool WindowGLFW::IsResizable() const
{
	return mSettings.mResizable;
}

void WindowGLFW::SetResizable(bool resizable)
{
	mSettings.mResizable = resizable;
	glfwSetWindowAttrib(mHWin, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
}

bool WindowGLFW::IsBorderless() const
{
	return mSettings.mBorderless;
}

void WindowGLFW::SetBorderless(bool borderless)
{
	mSettings.mBorderless = borderless;
	glfwSetWindowAttrib(mHWin, GLFW_DECORATED, borderless ? GLFW_FALSE : GLFW_TRUE);
}

bool WindowGLFW::IsFocused() const
{
	return mSettings.mFocused;
}

void WindowGLFW::SetFocus(bool focused)
{
	mSettings.mFocused = focused;
	if(focused)
		glfwFocusWindow(mHWin);
}

bool WindowGLFW::IsHidden() const
{
	return mSettings.mHidden;
}

void WindowGLFW::SetHidden(bool hidden)
{
	mSettings.mHidden = hidden;

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
