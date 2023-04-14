#include <Assert.hpp>

#include <Logger.hpp>

#ifndef DISABLE_ASSERTION

void OutputAssertionFailure(const char* expr, const char* msg, const char* file,
                            const int line)
{
	if(*msg)  // with message
	{
		FATAL("%s", msg);
	}
	else  // without message
	{
		Path p(file);
		FATAL("Assertion %s Failed at  %s:%s", expr,
		      p.filename().generic_string().c_str(), line);
	}
}

#endif
