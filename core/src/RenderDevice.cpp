#include <RenderDevice.hpp>

#include <Assert.hpp>
#include <RenderDeviceGL.hpp>
#include <WindowGLFW.hpp>

RenderAPI RenderDevice::sAPI = RenderAPI::GL;

Uptr<RenderDevice> RenderDevice::Create()
{
	switch(sAPI)
	{
	case RenderAPI::GL: return MakeUptr<RenderDeviceGL>();
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}
