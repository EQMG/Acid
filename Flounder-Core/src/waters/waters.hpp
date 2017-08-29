#pragma once

#include "../framework/framework.hpp"

#include "water.hpp"

namespace flounder
{
	/// <summary>
	/// A module used for managing water 3D worlds.
	/// </summary>
	class waters :
		public imodule
	{
	private:
		water *m_water;

		float m_colourIntensity; // 0 being 100% reflective, 1 disables reflections.
		bool m_enableReflections;
		float m_reflectionQuality;
		bool m_reflectionShadows;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline waters *get()
		{
			return static_cast<waters*>(framework::get()->getInstance("waters"));
		}

		/// <summary>
		/// Creates a new waters module.
		/// </summary>
		waters();

		/// <summary>
		/// Deconstructor for the waters module.
		/// </summary>
		~waters();

		void update() override;

		inline water *getWater() { return m_water; }

		inline void setWater(water *water) { m_water = water; }

		inline float getColourIntensity() const { return m_colourIntensity; }

		inline void setColourIntensity(const float &colourIntensity) { m_colourIntensity = colourIntensity; }

		inline bool getEnableReflections() const { return m_enableReflections; }

		inline void setEnableReflections(const bool &enableReflections) { m_enableReflections = enableReflections; }

		inline float getReflectionQuality() const { return m_reflectionQuality; }

		inline void setReflectionQuality(const float &reflectionQuality) { m_reflectionQuality = reflectionQuality; }

		inline bool getReflectionShadows() const { return m_reflectionShadows; }

		inline void setReflectionShadows(const bool &reflectionShadows) { m_reflectionShadows = reflectionShadows; }
	};
}
