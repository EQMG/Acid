#include "Touches.hpp"

namespace fl
{
	void CallbackTouch(WsiShell shell, uint32_t id, uint32_t x, uint32_t y, WsiAction action)
	{
		Touch touch = Touches::Get()->m_touches[id];
		touch.m_action = action;
		touch.m_x = static_cast<float>(x) / static_cast<float>(Display::Get()->GetWidth());
		touch.m_y = static_cast<float>(y) / static_cast<float>(Display::Get()->GetHeight());
	}

	Touches::Touches() :
		IModule(),
		m_touches(std::array<Touch, 10>())
	{
		// Sets the default state of the keys to released.
		for (unsigned int i = 0; i < 10; i++)
		{
			m_touches[i] = {};
			m_touches[i].m_id = i;
		}

		// Sets the touch callbacks.
		WsiShellCallbacks *callbacks;
		wsiGetShellCallbacks(Display::Get()->GetWsiShell(), &callbacks);
		callbacks->pfnTouch = CallbackTouch;
	}

	Touches::~Touches()
	{
	}

	void Touches::Update()
	{
	}

	bool Touches::GetTouch(const Vector2 &coord, const float &maxDistance)
	{
		for (auto &touch : m_touches)
		{
			if (touch.m_action != WSI_ACTION_RELEASE)
			{
				float distance = coord.Distance(Vector2(touch.m_x, touch.m_y));

				if (distance < maxDistance)
				{
					return true;
				}
			}
		}

		return false;
	}
}
