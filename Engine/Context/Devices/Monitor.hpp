#pragma once

#include "Maths/Vector2.hpp"
#include "ContextExport.hpp"

struct GLFWmonitor;

namespace acid {
/**
 * @brief Class used to describe the video mode for a monitor.
 */
class ACID_CONTEXT_EXPORT VideoMode {
public:
	/// The width, in screen coordinates, of the video mode.
	int32_t width;
	/// The height, in screen coordinates, of the video mode.
	int32_t height;
	/// The bit depth of the red channel of the video mode.
	int32_t redBits;
	/// The bit depth of the green channel of the video mode.
	int32_t greenBits;
	/// The bit depth of the blue channel of the video mode.
	int32_t blueBits;
	/// The refresh rate, in Hz, of the video mode.
	int32_t refreshRate;
};

/**
 * @brief Class used to describe the gamma ramp for a monitor.
 */
class ACID_CONTEXT_EXPORT GammaRamp {
public:
	/// An array of value describing the response of the red channel.
	uint16_t *red;
	/// An array of value describing the response of the green channel.
	uint16_t *green;
	/// An array of value describing the response of the blue channel.
	uint16_t *blue;
	/// The number of elements in each array.
	uint32_t size;
};

/**
 * @brief Class used to represent a monitor.
 */
class ACID_CONTEXT_EXPORT Monitor {
	friend class Window;
public:
	explicit Monitor(GLFWmonitor *monitor = nullptr);

	ACID_CONTEXT_NO_EXPORT GLFWmonitor *GetMonitor() const { return monitor; }

	bool IsPrimary() const;

	/**
	 * Gets the workarea size of the monitor.
	 * @return The size, in pixels, of the display area.
	 */
	Vector2ui GetWorkareaSize() const;

	/**
	 * Gets the workarea size of the monitor.
	 * @return The size, in pixels, of the display area.
	 */
	Vector2ui GetWorkareaPosition() const;

	/**
	 * Gets the physical size of the monitor.
	 * @return The size, in millimetres, of the display area.
	 */
	Vector2ui GetSize() const;

	/**
	 * Gets the content scale of the monitor.
	 * @return The content scale, the ratio between the current DPI and the platform's default DPI.
	 */
	Vector2f GetContentScale() const;

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
	GLFWmonitor *monitor;
};
}
