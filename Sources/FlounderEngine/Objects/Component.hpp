#pragma once

#include <string>
#include "../Prerequisites.hpp"

namespace Flounder
{
	class GameObject;

	class F_EXPORT Component
	{
	private:
		std::string m_name;
		GameObject *m_gameObject;
	public:
		Component();

		virtual ~Component();

		virtual std::string GetName() = 0;

		GameObject *GetGameObject() const { return m_gameObject; }

		void SetGameObject(GameObject *gameObject) { m_gameObject = gameObject; }
	};
}
