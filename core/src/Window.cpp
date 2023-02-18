#include <Window.hpp>

#include <Assert.hpp>
#include <RenderDevice.hpp>
#include <WindowGLFW.hpp>

WindowSettings::WindowSettings(const String& title, Vec2ui resolution,
                               Vec2ui position, WindowMode mode, VSyncMode vsync,
                               U32 msaa, bool srgb, bool resizable, bool borderless,
                               bool focused, bool hidden)
        : Title(title), Resolution(resolution), Position(position), Mode(mode),
          VSync(vsync), MSAA(msaa), SRGB(srgb), Resizable(resizable),
          Borderless(borderless), Focused(focused), Hidden(hidden)

{
}


Window::Window(const WindowSettings& settings): mSettings(settings)
{
}

WindowPtr Window::Create(const WindowSettings& settings)
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeUptr<WindowGLFW>(settings);
	}

	ASSERT(false, "Render API not supported");
	return nullptr;
}

bool Window::operator==(const Window& rhs) const
{
	return GetHandle() == rhs.GetHandle();
}

bool Window::operator!=(const Window& rhs) const
{
	return !operator==(rhs);
}
