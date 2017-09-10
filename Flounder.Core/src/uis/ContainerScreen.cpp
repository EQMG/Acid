#include "ContainerScreen.hpp"

namespace Flounder
{
	ContainerScreen::ContainerScreen(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, const bool &inScreenCoords) :
		UiObject(parent, position, dimensions)
	{
		SetInScreenCoords(inScreenCoords);
	}

	ContainerScreen::~ContainerScreen()
	{
	}

	void ContainerScreen::UpdateObject()
	{
	}
}
