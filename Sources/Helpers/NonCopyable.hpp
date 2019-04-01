#pragma once

#include "StdAfx.hpp"

namespace acid
{
/**
 * Class that makes derived classes non-copyable.
 */
class ACID_EXPORT NonCopyable
{
protected:
	NonCopyable() = default;

	virtual ~NonCopyable() = default;

public:
	NonCopyable(const NonCopyable &) = delete;

	NonCopyable &operator=(const NonCopyable &) = delete;
};
}
