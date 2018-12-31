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
	protected:
		std::string m_name;
	public:
		/// <summary>
		/// Creates a new resource with a unique name.
		/// </summary>
		/// <param name="name"> The unique name to use with this resource, this can be changed later. </param>
		IResource(const std::string &name) :
			m_name(name)
		{
		}

		/// <summary>
		/// Gets a unique name (most likely a filename) associated with this resource object.
		/// </summary>
		/// <returns> The objects unique name. </returns>
		virtual std::string GetName() const { return m_name; };
	};
}
