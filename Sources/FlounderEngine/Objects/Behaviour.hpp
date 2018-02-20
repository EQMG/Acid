#pragma once

#include "../Prerequisites.hpp"

namespace Flounder
{
	class F_EXPORT Behaviour
	{
	private:
	public:
		Behaviour();

		virtual ~Behaviour();

		virtual void Start();

		virtual void Update();

		virtual void OnEnable();

		virtual void OnDisable();
	};
}
