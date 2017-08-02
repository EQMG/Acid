#pragma once

#include "../maths/vector3.h"
#include "../maths/matrix4x4.h"
#include "../framework/framework.h"
#include "../skyboxes/skyboxes.h"
#include "../shadows/shadows.h"
#include "../visual/driverlinear.h"

namespace flounder
{
	/// <summary>
	/// A module used for managing worlds.
	/// </summary>
	class worlds :
		public imodule
	{
	private:
		driverlinear *m_driverDay;
		float m_factorDay;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static worlds *get()
		{
			return static_cast<worlds*>(framework::get()->getInstance("worlds"));
		}

		/// <summary>
		/// Creates a new worlds module.
		/// </summary>
		worlds();

		/// <summary>
		/// Deconstructor for the worlds module.
		/// </summary>
		~worlds();

		void update() override;

		float getDayFactor();

		float getSunriseFactor();

		float getShadowFactor();

		float getSunHeight();

		float starIntensity();
	};
}
