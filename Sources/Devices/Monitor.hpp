#pragma once

#include "Maths/Vector2.hpp"

struct GLFWmonitor;

namespace acid
{
/**
 * @brief Class used to describe the video mode for a monitor.
 */
class ACID_EXPORT VideoMode
{
public:
	/// The width, in screen coordinates, of the video mode.
	int32_t m_width;
	/// The height, in screen coordinates, of the video mode.
	int32_t m_height;
	/// The bit depth of the red channel of the video mode.
	int32_t m_redBits;
	/// The bit depth of the green channel of the video mode.
	int32_t m_greenBits;
	/// The bit depth of the blue channel of the video mode.
	int32_t m_blueBits;
	/// The refresh rate, in Hz, of the video mode.
	int32_t m_refreshRate;
};

/**
 * @brief Class used to describe the gamma ramp for a monitor.
 */
class ACID_EXPORT GammaRamp
{
public:
	/// An array of value describing the response of the red channel.
	uint16_t *m_red;
	/// An array of value describing the response of the green channel.
	uint16_t *m_green;
	/// An array of value describing the response of the blue channel.
	uint16_t *m_blue;
	/// The number of elements in each array.
	uint32_t m_size;
};

/**
 * @brief Class used to represent a monitor.
 */
class ACID_EXPORT Monitor
{
public:
	explicit Monitor(GLFWmonitor *monitor = nullptr);

	ACID_HIDDEN GLFWmonitor *GetMonitor() const { return m_monitor; }

	const bool &IsPrimary() const { return m_primary; }

	void SetPrimary(const bool &primary) { m_primary = primary; }

	/**
	 * Gets the physical size of the monitor.
	 * @return The size, in millimetres, of the display area.
	 */
	Vector2ui GetSize() const;

	/**
	 * Gets the position of the monitor's viewport on the virtual screen.
	 * @return The position of the monitor's viewport.
	 */
	Vector2ui GetPosition() const;

	/**
	 * Gets the name of this monitor.
	 * @return The name of the monitor.
	 */
	std::string GetName() const;

	/**
	 * Gets the available video modes for this monitor.
	 * @return The video modes supported by this monitor.
	 */
	std::vector<VideoMode> GetVideoModes() const;

	/**
	 * Gets the current mode of this monitor.
	 * @return The current video mode of this monitor.
	 */
	VideoMode GetVideoMode() const;

	/**
	 * Gets the current gamma ramp for this monitor.
	 * @return The current gamma ramp.
	 */
	GammaRamp GetGammaRamp() const;

	/**
	 * Sets the current gamma ramp for this monitor.
	 * @param gammaRamp The gamma ramp to use.
	 */
	void SetGammaRamp(const GammaRamp &gammaRamp) const;

private:
	GLFWmonitor *m_monitor;
	bool m_primary;
};
}
