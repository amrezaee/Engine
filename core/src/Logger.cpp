#include <Logger.hpp>

#if ENGINE_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

char Logger::sBuffer[1024] {};
char Logger::sFormatBuffer[512] {};

void Logger::Init()
{
	std::ios_base::sync_with_stdio(false);

#if ENGINE_PLATFORM_WINDOWS
	// Set output mode to handle virtual terminal sequences
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
	DWORD  dwMode  = 0;
	GetConsoleMode(hStdOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hStdOut, dwMode);

	dwMode = 0;
	GetConsoleMode(hStdErr, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hStdErr, dwMode);
#endif
}
