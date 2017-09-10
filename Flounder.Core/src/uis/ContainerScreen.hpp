#pragma once

#include "UiObject.hpp"

namespace Flounder
{
	/// <summary>
	/// A gui object that has a empty update and delete method.
	/// </summary>
	class ContainerScreen :
		public UiObject
	{
	public:
		ContainerScreen(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, const bool &inScreenCoords);

		~ContainerScreen();

		void UpdateObject() override;
	};
}
