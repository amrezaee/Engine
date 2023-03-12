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

// clang-format off
#include <entt/entt.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
//clang-format on

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T, typename... Args>
[[nodiscard]]
constexpr SharedPtr<T> MakeShared(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
[[nodiscard]]
constexpr UniquePtr<T> MakeUnique(Args&&... args)
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

using udword = uint64_t;
using uword  = uint32_t;
using uhalf  = uint16_t;
using byte   = uint8_t;
using uchar  = unsigned char;

using dword = int64_t;
using word  = int32_t;
using half  = int16_t;

using real = double;

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

	uword major {0};
	uword minor {0};
	uword release {0};
	uword build {0};
};

template<typename T, size_t size>
constexpr size_t ArraySize(T (&)[size])
{
	return size;
}
