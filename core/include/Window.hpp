#pragma once

#include <Common.hpp>
#include <InputMap.hpp>
#include <Signal.hpp>
#include <Vector2.hpp>

using WindowPtr = UniquePtr<class Window>;

enum class WindowMode
{
	Windowed,      // Windowed
	BorderlessFS,  // Borderless Fullscreen
	ExclusiveFS    // Exclusive Fullscreen
};

enum class VSyncMode : i32
{
	Immediate    = 0,
	Synchronized = 1,
	Adaptive     = -1
};

class WindowSettings
{
public:
	WindowSettings() = default;
	explicit WindowSettings(String title);
	WindowSettings(String     title,
	               vec2ui     resolution,
	               vec2ui     position,
	               WindowMode mode,
	               VSyncMode  vsync,
	               u32        msaa,
	               bool       srgb,
	               bool       resizable,
	               bool       borderless,
	               bool       focused,
	               bool       hidden);

	String     Title {"Engine"};
	vec2ui     Resolution {960, 540};
	vec2ui     Position {5, 40};
	WindowMode Mode {WindowMode::Windowed};
	VSyncMode  VSync {VSyncMode::Immediate};
	u32        MSAA {1};
	bool       SRGB {false};
	bool       Resizable {true};
	bool       Borderless {false};
	bool       Focused {true};
	bool       Hidden {false};
};

class Window
{
public:
	Signal<vec2ui>                   FramebufferSignal;
	Signal<vec2ui>                   SizeSignal;
	Signal<vec2ui>                   PositionSignal;
	Signal<>                         CloseSignal;
	Signal<bool>                     FocusSignal;
	Signal<Key, Key>                 KeySignal;
	Signal<MouseButton, MouseButton> MouseSignal;
	Signal<vec2>                     CursorSignal;
	Signal<vec2>                     ScrollSignal;

public:
	explicit Window(WindowSettings settings);
	Window(const Window&)            = delete;
	Window& operator=(const Window&) = delete;
	virtual ~Window()                = default;

	static WindowPtr Create(const WindowSettings& settings);

	virtual const String& GetTitle() const              = 0;
	virtual void          SetTitle(const String& title) = 0;

	// Client area width and height
	virtual u32    GetWidth() const           = 0;
	virtual u32    GetHeight() const          = 0;
	virtual vec2ui GetResolution() const      = 0;
	virtual void   SetWidth(u32 width)        = 0;
	virtual void   SetHeight(u32 height)      = 0;
	virtual void   SetResolution(vec2ui size) = 0;

	// Top left x and y position
	virtual vec2ui GetPosition() const     = 0;
	virtual void   SetPosition(vec2ui pos) = 0;

	// Windowed, BorderlessFullscreen or ExclusiveFullscreen
	virtual WindowMode GetWindowMode() const          = 0;
	virtual bool       IsFullscreen() const           = 0;
	virtual void       SetWindowMode(WindowMode mode) = 0;

	virtual VSyncMode GetVSyncMode() const         = 0;
	virtual void      SetVSyncMode(VSyncMode mode) = 0;

	// Multisampling anti aliasing (e.g. 1, 2, 4, 8, 16 and 32)
	virtual u32  GetMSAA() const      = 0;
	virtual void SetMSAA(u32 samples) = 0;

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

	bool operator==(const Window& rhs) const;
	bool operator!=(const Window& rhs) const;

protected:
	WindowSettings mSettings;
};
