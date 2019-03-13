#pragma once

#include "Maths/Vector3.hpp"

namespace acid
{
class ACID_EXPORT IVertex
{
  public:
	virtual ~IVertex() = default;

	virtual const Vector3& GetPosition() const = 0;

	virtual void SetPosition(const Vector3& position) = 0;
};
}
