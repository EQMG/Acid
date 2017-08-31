#pragma once

#include "../engine/Engine.hpp"

#include "water.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing water 3D worlds.
	/// </summary>
	class waters :
		public IModule
	{
	private:
		water *m_water;

		float m_colourIntensity; // 0 being 100% reflective, 1 disables reflections.
		bool m_enableReflections;
		float m_reflectionQuality;
		bool m_reflectionShadows;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static waters *get()
		{
			return static_cast<waters*>(Engine::Get()->GetModule("waters"));
		}

		/// <summary>
		/// Creates a new waters module.
		/// </summary>
		waters();

		/// <summary>
		/// Deconstructor for the waters module.
		/// </summary>
		~waters();

		void Update() override;

		water *getWater() { return m_water; }

		void setWater(water *water) { m_water = water; }

		float getColourIntensity() const { return m_colourIntensity; }

		void setColourIntensity(const float &colourIntensity) { m_colourIntensity = colourIntensity; }

		bool getEnableReflections() const { return m_enableReflections; }

		void setEnableReflections(const bool &enableReflections) { m_enableReflections = enableReflections; }

		float getReflectionQuality() const { return m_reflectionQuality; }

		void setReflectionQuality(const float &reflectionQuality) { m_reflectionQuality = reflectionQuality; }

		bool getReflectionShadows() const { return m_reflectionShadows; }

		void setReflectionShadows(const bool &reflectionShadows) { m_reflectionShadows = reflectionShadows; }
	};
}
