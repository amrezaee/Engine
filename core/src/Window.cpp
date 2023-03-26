#include <Window.hpp>

#include <Assert.hpp>
#include <RenderDevice.hpp>
#include <WindowGLFW.hpp>

WindowSettings::WindowSettings(String title): Title(std::move(title))
{
}

WindowSettings::WindowSettings(String title, Vec2ui resolution, Vec2ui position,
                               WindowMode mode, VSyncMode vsync, uword msaa,
                               bool srgb, bool resizable, bool borderless,
                               bool focused, bool hidden)
        : Title(std::move(title)), Resolution(resolution), Position(position),
          Mode(mode), VSync(vsync), MSAA(msaa), SRGB(srgb), Resizable(resizable),
          Borderless(borderless), Focused(focused), Hidden(hidden)

{
}


Window::Window(WindowSettings settings): mSettings(std::move(settings))
{
}

WindowPtr Window::Create(const WindowSettings& settings)
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeUnique<WindowGLFW>(settings);
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
