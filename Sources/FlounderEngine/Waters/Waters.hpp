#pragma once

#include "../Engine/Engine.hpp"
#include "WaterRender.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing water 3D worlds.
	/// </summary>
	class F_EXPORT Waters :
		public IModule
	{
	private:
		float m_colourIntensity;
		bool m_enableReflections;
		float m_reflectionQuality;
		bool m_reflectionShadows;
	public:
		static Waters *S_INSTANCE;

		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Waters *Get()
		{
			return S_INSTANCE; // reinterpret_cast<Waters *>(Engine::Get()->GetModule("waters"));
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
