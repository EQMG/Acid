#include "ContainerScreen.hpp"

namespace Flounder
{
	ContainerScreen::ContainerScreen(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, const bool &inScreenCoords) :
		UiObject(parent, position, dimensions, Vector2(0.5f, 0.5f), inScreenCoords)
	{
	}

	ContainerScreen::~ContainerScreen()
	{
	}

	void ContainerScreen::UpdateObject()
	{
	}
}
