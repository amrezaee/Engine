#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#define UNUSED(x) ((void)x)
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


template<typename T>
using Sptr = std::shared_ptr<T>;

template<typename T>
using Uptr = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr Sptr<T> MakeSptr(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
constexpr Uptr<T> MakeUptr(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Vector = std::vector<T>;

template<typename T1, typename T2>
using HashMap = std::unordered_map<T1, T2>;

using String      = std::string;
using StringArray = std::vector<std::string>;
using Path        = std::filesystem::path;

namespace fs = std::filesystem;

using U64 = uint64_t;
using U32 = uint32_t;
using U16 = uint16_t;
using U8  = uint8_t;

using I64 = int64_t;
using I32 = int32_t;
using I16 = int16_t;
using I8  = int8_t;

// use this only in public section of class
#define PREVENT_COPY(cls)                \
	cls& operator=(const cls&) = delete; \
	cls(const cls&)            = delete

struct Version
{
	Version() = default;
	String GetString() const
	{
		String r;

		r = std::to_string(major) + '.' + std::to_string(minor) + '.' +
		    std::to_string(release) + '.' + std::to_string(build);
		return r;
	}

	U32 major {0};
	U32 minor {0};
	U32 release {0};
	U32 build {0};
};

template<typename T, size_t size>
constexpr size_t ArraySize(T (&)[size])
{
	return size;
}
