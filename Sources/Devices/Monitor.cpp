#include "Monitor.hpp"

#include "Window.hpp"
#include <GLFW/glfw3.h>

namespace acid
{
Monitor::Monitor(GLFWmonitor* monitor) : m_monitor(monitor), m_primary(m_monitor == glfwGetPrimaryMonitor()) {}

Vector2 Monitor::GetPosition() const
{
	int32_t xpos;
	int32_t ypos;
	glfwGetMonitorPos(m_monitor, &xpos, &ypos);
	return Vector2(xpos, ypos);
}

Vector2 Monitor::GetDimensions() const
{
	int32_t widthMM;
	int32_t heightMM;
	glfwGetMonitorPhysicalSize(m_monitor, &widthMM, &heightMM);
	return Vector2(widthMM, heightMM);
}

std::string Monitor::GetName() const
{
	return glfwGetMonitorName(m_monitor);
}

std::vector<VideoMode> Monitor::GetVideoModes() const
{
	int32_t videoModeCount;
	const GLFWvidmode* videoModes = glfwGetVideoModes(m_monitor, &videoModeCount);
	std::vector<VideoMode> modes(static_cast<uint32_t>(videoModeCount));

	for(uint32_t i = 0; i < static_cast<uint32_t>(videoModeCount); i++)
		{
			modes[i] = *reinterpret_cast<const VideoMode*>(&videoModes[i]);
		}

	return modes;
}

VideoMode Monitor::GetVideoMode() const
{
	const GLFWvidmode* videoMode = glfwGetVideoMode(m_monitor);
	return *reinterpret_cast<const VideoMode*>(videoMode);
}

GammaRamp Monitor::GetGammaRamp() const
{
	const GLFWgammaramp* gamaRamp = glfwGetGammaRamp(m_monitor);
	return *reinterpret_cast<const GammaRamp*>(gamaRamp);
}

void Monitor::SetGammaRamp(const GammaRamp& gammaRamp) const
{
	auto ramp = reinterpret_cast<const GLFWgammaramp*>(&gammaRamp);
	glfwSetGammaRamp(m_monitor, ramp);
}
}
