#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <functional>
#include <future>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std::chrono_literals;

#include "acid_export.h"

// TODO: Use cr.h state defines.
#if defined(ACID_BUILD_MSVC)
#	pragma section(".state", read, write)
#	define ACID_STATE __declspec(allocate(".state"))
#else
#	if defined(ACID_BUILD_MACOS)
#		define ACID_STATE __attribute__((used, section("__DATA,__state")))
#	else
#		pragma section(".state", read, write)
#		define ACID_STATE __attribute__((section(".state")))
#	endif
#endif
