#include "Sandbox.hpp"

Uptr<Application> CreateApp()
{
	return MakeUptr<Sandbox>();
}