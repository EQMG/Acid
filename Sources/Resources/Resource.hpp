#pragma once

#include "StdAfx.hpp"

namespace acid
{
class Metadata;

/**
 * A managed resource object. Implementations contain Create functions that can take a metadata object or pass parameters to the constructor.
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

	/**
	 * Used to decode this resource from a loaded data format.
	 * @param metadata The metadata to decode from.
	 */
	virtual void Decode(const Metadata &metadata)
	{
	}

	/**
	 * Used to encode this resource into a data format.
	 * @param metadata The metadata to encode into.
	 */
	virtual void Encode(Metadata &metadata) const
	{
	}
};
}
