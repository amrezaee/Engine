#pragma once

#include <Common.hpp>

#ifndef DISABLE_ASSERTION

void OutputAssertionFailure(const char* expr, const char* msg, const char* file,
                            const int line);

	#define ASSERT_IMPL_(expr)                                             \
		{                                                                  \
			if(!(expr))                                                    \
			{                                                              \
				OutputAssertionFailure(STR(expr), "", __FILE__, __LINE__); \
				DEBUGBREAK();                                              \
			}                                                              \
		}

	#define ASSERT_MSG_IMPL_(expr, msg)                                       \
		{                                                                     \
			if(!(expr))                                                       \
			{                                                                 \
				OutputAssertionFailure(STR(expr), (msg), __FILE__, __LINE__); \
				DEBUGBREAK();                                                 \
			}                                                                 \
		}

	#define GET_MACRO_NAME_(_1, _2, macro, ...) macro
	#define GET_MACRO_(...) \
		EXPAND(GET_MACRO_NAME_(__VA_ARGS__, ASSERT_MSG_IMPL_, ASSERT_IMPL_))

	#define ASSERT(...) EXPAND(GET_MACRO_(__VA_ARGS__)(__VA_ARGS__))

#else
	#define ASSERT(...) ((void)0)
#endif
