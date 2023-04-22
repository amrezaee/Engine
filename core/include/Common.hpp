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
// clang-format on

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T, typename... Args>
[[nodiscard]] constexpr SharedPtr<T> MakeShared(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
[[nodiscard]] constexpr UniquePtr<T> MakeUnique(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Vector = std::vector<T>;

template<typename T>
using List = std::list<T>;

template<typename T1, typename T2>
using HashMap = std::unordered_map<T1, T2>;

using String      = std::string;
using StringArray = std::vector<std::string>;
using Path        = std::filesystem::path;

namespace fs = std::filesystem;

using u64   = uint64_t;
using u32   = uint32_t;
using u16   = uint16_t;
using u8    = uint8_t;
using usize = size_t;

using i64   = int64_t;
using i32   = int32_t;
using i16   = int16_t;
using i8    = int8_t;
using isize = long long;

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

	u32 major {0};
	u32 minor {0};
	u32 release {0};
	u32 build {0};
};
