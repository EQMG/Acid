#include "Touches.hpp"

namespace fl
{
	void CallbackTouch(uint32_t id, float x, float y, bool isDown)
	{
		Touch touch = Touches::Get()->m_touches[id];
		touch.m_isDown = isDown;
		touch.m_x = x;
		touch.m_y = y;
	}

	Touches::Touches() :
		IModule(),
		m_touches(std::array<Touch, 10>())
	{
		auto shell = Display::Get()->GetShell();

		// Sets the default state of the keys to released.
		for (unsigned int i = 0; i < 10; i++)
		{
			m_touches[i] = {};
			m_touches[i].m_id = i;
		}

		// Sets the touch callbacks.
		shell->SetCallbackTouch(CallbackTouch);
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
			if (touch.m_isDown)
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
