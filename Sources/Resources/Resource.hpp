#pragma once

#include "StdAfx.hpp"

namespace acid
{
class Metadata;

/**
 * @brief A managed resource object. Implementations contain Create functions that can take a metadata object or pass parameters to the constructor.
 */
class ACID_EXPORT Resource
{
public:
	Resource() = default;

	virtual ~Resource() = default;

	/**
	 * Used by the resource after it has been decoded, and in constructors.
	 */
	virtual void Load()
	{
	}
};
}
