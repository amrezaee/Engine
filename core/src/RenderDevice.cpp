#include <RenderDevice.hpp>

#include <Assert.hpp>
#include <RenderDeviceGL.hpp>

RenderAPI RenderDevice::sAPI = RenderAPI::GL;

RenderDevicePtr RenderDevice::Create()
{
	switch(sAPI)
	{
	case RenderAPI::GL: return MakeUnique<RenderDeviceGL>();
	}
	ASSERT(false, "Render API not supported");
	return nullptr;
}

RenderAPI RenderDevice::GetAPI()
{
	return sAPI;
}

const RenderDeviceInfo& RenderDevice::GetInfo() const
{
	return mInfo;
}
