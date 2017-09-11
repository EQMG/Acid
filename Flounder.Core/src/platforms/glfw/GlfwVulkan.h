#pragma once

#ifdef FLOUNDER_PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb/stb_image.h"
