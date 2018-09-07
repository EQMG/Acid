#pragma once

#include <vector>
#include "Maths/Vector3.hpp"

namespace acid
{
	class ACID_EXPORT IVertex
	{
	public:
		virtual Vector3 GetPosition() const = 0;

		virtual void SetPosition(const Vector3 &position) = 0;

		virtual size_t GetSize() const = 0;
	};
}
