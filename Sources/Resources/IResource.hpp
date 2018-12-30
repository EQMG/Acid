#pragma once

#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// A managed resource object.
	/// </summary>
	class ACID_EXPORT IResource
	{
	public:
		/// <summary>
		/// Gets a unique name (most likely a filename) associated with this resource object.
		/// </summary>
		/// <returns> The objects unique name. </returns>
		virtual std::string GetFilename() const = 0;
	};
}
