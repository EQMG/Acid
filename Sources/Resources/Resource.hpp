#pragma once

#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief A managed resource object. Implementations contain Create functions that can take a node object or pass parameters to the constructor.
 */
class ACID_EXPORT Resource
{
public:
	Resource() = default;

	virtual ~Resource() = default;
};
}
