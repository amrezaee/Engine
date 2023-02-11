#pragma once

#include <Common.hpp>

#include <Signal.hpp>
#include <Vector2.hpp>

enum class WindowMode
{
	Windowed,      // Windowed
	BorderlessFS,  // Borderless Fullscreen
	ExclusiveFS    // Exclusive Fullscreen
};

enum class VSyncMode : I32
{
	Immediate    = 0,
	Synchronized = 1,
	Adaptive     = -1
};

class WindowSettings
{
public:
	WindowSettings() = default;
	WindowSettings(const String& title, Vec2ui resolution, Vec2ui position,
	               WindowMode mode = WindowMode::Windowed,
	               VSyncMode vsync = VSyncMode::Immediate, U32 msaa = 1,
	               bool srgb = false, bool resizable = true, bool borderless = false,
	               bool focused = false, bool hidden = false)
	        : mTitle(title), mResolution(resolution), mPosition(position),
	          mWindowMode(mode), mVSyncMode(vsync), mMSAA(msaa), mSRGB(srgb),
	          mResizable(resizable), mBorderless(borderless), mFocused(focused),
	          mHidden(hidden)

	{
	}

	String     mTitle {"app"};
	Vec2ui     mResolution {640, 480};
	Vec2ui     mPosition;
	WindowMode mWindowMode {WindowMode::Windowed};
	VSyncMode  mVSyncMode {VSyncMode::Immediate};
	U32        mMSAA {1};
	bool       mSRGB {false};
	bool       mResizable {true};
	bool       mBorderless {false};
	bool       mFocused {true};
	bool       mHidden {false};
};


class Window
{
public:
	Signal<Vec2ui> FramebufferSignal;
	Signal<Vec2ui> SizeSignal;
	Signal<Vec2ui> PositionSignal;
	Signal<>       CloseSignal;
	Signal<bool>   FocusSignal;

public:
	PREVENT_COPY(Window);
	Window(const WindowSettings& settings);
	virtual ~Window() = default;

	static Uptr<Window> Create(const WindowSettings& settings);

	virtual void Destroy() = 0;

	virtual const String& GetTitle() const              = 0;
	virtual void          SetTitle(const String& title) = 0;

	// Client area width and height
	virtual U32    GetWidth() const           = 0;
	virtual U32    GetHeight() const          = 0;
	virtual Vec2ui GetResolution() const      = 0;
	virtual void   SetWidth(U32 width)        = 0;
	virtual void   SetHeight(U32 height)      = 0;
	virtual void   SetResolution(Vec2ui size) = 0;

	// Top left x and y position
	virtual Vec2ui GetPosition() const     = 0;
	virtual void   SetPosition(Vec2ui pos) = 0;

	// Windowed, BorderlessFullscreen or ExclusiveFullscreen
	virtual WindowMode GetWindowMode() const          = 0;
	virtual bool       IsFullscreen() const           = 0;
	virtual void       SetWindowMode(WindowMode mode) = 0;

	virtual VSyncMode GetVSyncMode() const         = 0;
	virtual void      SetVSyncMode(VSyncMode mode) = 0;

	// Multisampling anti aliasing (e.g. 1, 2, 4, 8, 16 and 32)
	virtual U32  GetMSAA() const      = 0;
	virtual void SetMSAA(U32 samples) = 0;

	virtual bool IsSRGB() const     = 0;
	virtual void SetSRGB(bool srgb) = 0;

	virtual bool IsResizable() const          = 0;
	virtual void SetResizable(bool resizable) = 0;

	virtual bool IsBorderless() const           = 0;
	virtual void SetBorderless(bool borderless) = 0;

	virtual bool IsFocused() const      = 0;
	virtual void SetFocus(bool focused) = 0;

	virtual bool IsHidden() const       = 0;
	virtual void SetHidden(bool hidden) = 0;

	virtual void* GetHandle() const = 0;

	virtual void SwapBuffers() = 0;
	virtual void PollEvents()  = 0;

	bool operator==(const Window& rhs) const
	{
		return GetHandle() == rhs.GetHandle();
	}

	bool operator!=(const Window& rhs) const { return !operator==(rhs); }

protected:
	WindowSettings mSettings;
};
