#include "Monitor.hpp"

#include <GLFW/glfw3.h>
#include "Window.hpp"

namespace acid
{
Monitor::Monitor(GLFWmonitor *monitor) :
	m_monitor(monitor),
	m_primary(m_monitor == glfwGetPrimaryMonitor())
{
}

Vector2ui Monitor::GetSize() const
{
	int32_t widthMM;
	int32_t heightMM;
	glfwGetMonitorPhysicalSize(m_monitor, &widthMM, &heightMM);
	return Vector2ui(widthMM, heightMM);
}

Vector2ui Monitor::GetPosition() const
{
	int32_t xpos;
	int32_t ypos;
	glfwGetMonitorPos(m_monitor, &xpos, &ypos);
	return Vector2ui(xpos, ypos);
}

std::string Monitor::GetName() const
{
	return glfwGetMonitorName(m_monitor);
}

std::vector<VideoMode> Monitor::GetVideoModes() const
{
	int32_t videoModeCount;
	const GLFWvidmode *videoModes = glfwGetVideoModes(m_monitor, &videoModeCount);
	std::vector<VideoMode> modes(static_cast<uint32_t>(videoModeCount));

	for (uint32_t i = 0; i < static_cast<uint32_t>(videoModeCount); i++)
	{
		modes[i] = *reinterpret_cast<const VideoMode *>(&videoModes[i]);
	}

	return modes;
}

VideoMode Monitor::GetVideoMode() const
{
	const GLFWvidmode *videoMode = glfwGetVideoMode(m_monitor);
	return *reinterpret_cast<const VideoMode *>(videoMode);
}

GammaRamp Monitor::GetGammaRamp() const
{
	const GLFWgammaramp *gamaRamp = glfwGetGammaRamp(m_monitor);
	return *reinterpret_cast<const GammaRamp *>(gamaRamp);
}

void Monitor::SetGammaRamp(const GammaRamp &gammaRamp) const
{
	auto ramp = reinterpret_cast<const GLFWgammaramp *>(&gammaRamp);
	glfwSetGammaRamp(m_monitor, ramp);
}
}
