#pragma once

#ifndef DISABLE_ASSERTION

	#define EXPAND(x) x
	#define STR(s)    #s

	#if defined(_MSC_VER)
extern void __cdecl __debugbreak(void);
		#define DEBUGBREAK() __debugbreak()

	#elif((!defined(__NACL__)) && ((defined(__GNUC__) || defined(__clang__)) && \
	                               (defined(__i386__) || defined(__x86_64__))))
		#define DEBUGBREAK() __asm__ __volatile__("int $3\n\t")

	#elif(defined(__APPLE__) && (defined(__arm64__) || defined(__aarch64__)))
		#define DEBUGBREAK() __asm__ __volatile__("brk #22\n\t")

	#elif defined(__APPLE__) && defined(__arm__)
		#define DEBUGBREAK() __asm__ __volatile__("bkpt #22\n\t")

	#elif defined(__386__) && defined(__WATCOMC__)
		#define DEBUGBREAK()      \
			{                     \
				_asm { int 0x03 } \
			}

	#elif defined(HAVE_SIGNAL_H) && !defined(__WATCOMC__)
		#include <signal.h>
		#define DEBUGBREAK() raise(SIGTRAP)

	#else
		#define DEBUGBREAK() ((void)0)
	#endif


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
