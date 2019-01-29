#pragma once

#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	class Metadata;

	/// <summary>
	/// A managed resource object.
	/// </summary>
	class ACID_EXPORT Resource
	{
	public:
		/// <summary>
		/// Creates a new resource.
		/// </summary>
		explicit Resource()
		{
		}

		/// <summary>
		/// Used to encode this resource into a data format.
		/// </summary>
		/// <param name="metadata"> The metadata to encode into. </param>
		virtual void Encode(Metadata &metadata) const
		{
		}
	};
}
