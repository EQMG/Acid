#pragma once

#include "../maths/vector3.hpp"
#include "../maths/matrix4x4.hpp"
#include "../framework/framework.hpp"
#include "../skyboxes/skyboxes.hpp"
#include "../shadows/shadows.hpp"
#include "../visual/driverlinear.hpp"
#include "../noise/noisefast.hpp"

namespace flounder
{
	/// <summary>
	/// A module used for managing worlds.
	/// </summary>
	class worlds :
		public imodule
	{
	private:
		noisefast *m_noise;

		driverlinear *m_driverDay;
		float m_factorDay;

		vector3 *m_sunPosition;
		colour *m_sunColour;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline worlds *get()
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

		noisefast *getNoise() const { return m_noise; }

		float getDayFactor();

		float getSunriseFactor();

		float getShadowFactor();

		float getSunHeight();

		float starIntensity();

		vector3 *getSunPosition() const { return m_sunPosition; }

		colour *getSunColour() const { return m_sunColour; }
	};
}
