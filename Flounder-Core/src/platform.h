#pragma once

// Graphics API.
#ifdef FLOUNDER_API_GL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#elif FLOUNDER_API_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#elif FLOUNDER_API_WEB
#define GLFW_INCLUDE_ES3
#define GLFW_FALSE false
#define GLFW_TRUE true
#include <emscripten/emscripten.h>
#include <GLFW/glfw3.h>
#endif

// Audio API.
#ifdef FLOUNDER_API_AL
//#include <AL/al.h>
//#include <AL/alc.h>
#elif FLOUNDER_API_WEB
#include <emscripten/emscripten.h>
#endif