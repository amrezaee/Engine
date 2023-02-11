#include <Application.hpp>

int main(int argc, char** argv)
{
	Logger::Init();
	TRACE("Engine starting");

	auto app = CreateApp();
	app->Run();

	TRACE("Engine exiting");
	return 0;
}