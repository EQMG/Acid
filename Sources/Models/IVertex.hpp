#pragma once

#include <vector>
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"

namespace fl
{
	class FL_EXPORT IVertex
	{
	public:
		IVertex()
		{
		}

		virtual ~IVertex()
		{
		}

		virtual Vector3 GetPosition() const = 0;

		virtual void SetPosition(const Vector3 &position) = 0;

		virtual size_t GetSize() const = 0;

		virtual void *GetData(std::vector<IVertex *> &vertices) = 0;
	};
}