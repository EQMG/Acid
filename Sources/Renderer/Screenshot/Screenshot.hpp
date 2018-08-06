#pragma once

#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	class ACID_EXPORT Screenshot
	{
	public:
		/// <summary>
		/// Takes a screenshot of the current image of the display and saves it into a image file.
		/// </summary>
		///	<param name="filename"> The file to save the screenshot to. </param>
		static void Capture(const std::string &filename);
	};
}
