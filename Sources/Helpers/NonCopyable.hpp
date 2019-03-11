#pragma once

#include "AcidPch.hpp"

namespace acid
{
	/// <summary>
	/// A class that makes derived classes non-copyable.
	/// </summary>
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
