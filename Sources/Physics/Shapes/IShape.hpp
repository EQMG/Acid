#pragma once

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include "Files/LoadedValue.hpp"

namespace fl
{
	/// <summary>
	/// A simple class that represents a physics shape.
	/// </summary>
	class FL_EXPORT IShape
	{
	public:
		/// <summary>
		/// Creates a new shape.
		/// </summary>
		IShape()
		{
		}

		/// <summary>
		/// Deconstructor for the shape.
		/// </summary>
		virtual ~IShape()
		{
		}

		virtual void Load(LoadedValue *value) = 0;

		virtual void Write(LoadedValue *destination) = 0;

		virtual std::string GetName() const = 0;

		virtual btCollisionShape *GetCollisionShape() const = 0;
	};
}
