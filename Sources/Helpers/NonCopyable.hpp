#pragma once

#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// A class that makes derived classes non-copyable.
	/// </summary>
	class ACID_EXPORT NonCopyable
	{
	protected:
		NonCopyable() = default;

		~NonCopyable() = default;
	public:
		NonCopyable(const NonCopyable &) = delete;

		NonCopyable &operator=(const NonCopyable &) = delete;
	};
}
