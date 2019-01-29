#pragma once

#include <string>
#include "Serialized/Metadata.hpp"

namespace acid
{
	/// <summary>
	/// A managed resource object.
	/// </summary>
	class ACID_EXPORT Resource
	{
	public:
		/// <summary>
		/// Creates a new resource with a unique name.
		/// </summary>
		/// <param name="name"> The unique name to use with this resource, this can be changed later. </param>
		explicit Resource(const std::string &name) :
			m_name(name)
		{
		}

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

		/// <summary>
		/// Gets a unique name (most likely a filename) associated with this resource object.
		/// </summary>
		/// <returns> The objects unique name. </returns>
		virtual const std::string &GetName() const { return m_name; };
	protected:
		std::string m_name;
	};
}
