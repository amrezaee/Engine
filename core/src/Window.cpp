#include <Window.hpp>

#include <Assert.hpp>
#include <RenderDevice.hpp>
#include <WindowGLFW.hpp>

Window::Window(const WindowSettings& settings): mSettings(settings)
{
}

Uptr<Window> Window::Create(const WindowSettings& settings)
{
	switch(RenderDevice::GetAPI())
	{
	case RenderAPI::GL: return MakeUptr<WindowGLFW>(settings);
	}

	ASSERT(false, "Render API not supported");
	return nullptr;
}