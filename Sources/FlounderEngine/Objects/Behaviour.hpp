#pragma once

#include "../Prerequisites.hpp"
#include "Component.hpp"

namespace Flounder
{
	class F_EXPORT Behaviour :
		public Component
	{
	private:
		bool m_enabled;
	public:
		Behaviour(const bool &enabled = false);

		virtual ~Behaviour();

		virtual void Update() override;

		virtual void OnEnable();

		virtual void OnDisable();

		void SetEnabled(const bool &enable);

		std::string GetName() const override { return "Behaviour"; };
	};
}
