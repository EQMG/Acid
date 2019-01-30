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
	protected:
		Resource() = default;

		~Resource() = default;

		/// <summary>
		/// Used by the resource after it has been decoded, and in constructors.
		/// </summary>
		virtual void Load()
		{
		}
	public:
		/// <summary>
		/// Used to decode this resource from a loaded data format.
		/// </summary>
		/// <param name="metadata"> The metadata to decode from. </param>
		virtual void Decode(const Metadata &metadata)
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
