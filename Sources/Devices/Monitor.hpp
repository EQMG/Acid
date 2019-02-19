#pragma once

#include <vector>
#include "Maths/Vector2.hpp"

struct GLFWmonitor;

namespace acid
{
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

	/// <summary>
	/// A class used to represent a monitor.
	/// </summary>
	class ACID_EXPORT Monitor
	{
	public:
		explicit Monitor(GLFWmonitor *monitor);

		ACID_HIDDEN GLFWmonitor *GetMonitor() const { return m_monitor; }

		const bool &IsPrimary() const { return m_primary; }

		void SetPrimary(const bool &primary) { m_primary = primary; }

		/// <summary>
		/// Gets the position of the monitor's viewport on the virtual screen.
		/// </summary>
		/// <returns> The position of the monitor's viewport. </returns>
		Vector2 GetPosition() const;

		/// <summary>
		/// Gets the physical size of the monitor.
		/// </summary>
		/// <returns> The size, in millimetres, of the display area. </returns>
		Vector2 GetDimensions() const;

		/// <summary>
		/// Gets the name of this monitor.
		/// </summary>
		/// <returns> The human-readable name, encoded as UTF-8. </returns>
		std::string GetName() const;

		/// <summary>
		/// Gets the available video modes for this monitor.
		/// </summary>
		/// <returns> The video modes supported by this monitor. </returns>
		std::vector<VideoMode> GetVideoModes() const;

		/// <summary>
		/// Gets the current mode of this monitor.
		/// </summary>
		/// <returns> The current video mode of this monitor. </returns>
		VideoMode GetVideoMode() const;

		/// <summary>
		/// Gets the current gamma ramp for this monitor.
		/// </summary>
		/// <returns> The current gamma ramp. </returns>
		GammaRamp GetGammaRamp() const;

		/// <summary>
		/// Sets the current gamma ramp for this monitor.
		/// </summary>
		/// <param name="gammaRamp"> The gamma ramp to use. </param>
		void SetGammaRamp(const GammaRamp &gammaRamp) const;

	private:
		GLFWmonitor *m_monitor;
		bool m_primary;
	};
}
