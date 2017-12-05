#pragma once

#include "../engine/Engine.hpp"
#include "Water.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing water 3D worlds.
	/// </summary>
	class Waters :
		public IModule
	{
	private:
		Water *m_water;

		float m_colourIntensity; // 0 being 100% reflective, 1 disables reflections.
		bool m_enableReflections;
		float m_reflectionQuality;
		bool m_reflectionShadows;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Waters *Get()
		{
			return static_cast<Waters*>(Engine::Get()->GetModule("waters"));
		}

		/// <summary>
		/// Creates a new waters module.
		/// </summary>
		Waters();

		/// <summary>
		/// Deconstructor for the waters module.
		/// </summary>
		~Waters();

		void Update() override;

		Water *GetWater() const { return m_water; }

		void SetWater(Water *water) { m_water = water; }

		float GetColourIntensity() const { return m_colourIntensity; }

		void SetColourIntensity(const float &colourIntensity) { m_colourIntensity = colourIntensity; }

		bool GetEnableReflections() const { return m_enableReflections; }

		void SetEnableReflections(const bool &enableReflections) { m_enableReflections = enableReflections; }

		float GetReflectionQuality() const { return m_reflectionQuality; }

		void SetReflectionQuality(const float &reflectionQuality) { m_reflectionQuality = reflectionQuality; }

		bool GetReflectionShadows() const { return m_reflectionShadows; }

		void SetReflectionShadows(const bool &reflectionShadows) { m_reflectionShadows = reflectionShadows; }
	};
}
