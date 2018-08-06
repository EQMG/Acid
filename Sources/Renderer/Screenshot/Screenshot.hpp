#pragma once

#include <string>
#include "Display/Display.hpp"

namespace acid
{
	class ACID_EXPORT Screenshot
	{
	public:
		/// <summary>
		/// Takes a screenshot of the current image of the display and saves it into the screenshots folder a png image.
		/// </summary>
		///	<param name="filename"> The new width in pixels. </param>
		static void Capture(const std::string &filename);
	};
}
