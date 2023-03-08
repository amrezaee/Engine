#include "Sandbox.hpp"

UniquePtr<Application> CreateApp()
{
	return MakeUnique<Sandbox>();
}