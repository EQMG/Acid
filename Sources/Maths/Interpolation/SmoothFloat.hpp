#pragma once

#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// A class that smoothly increases its value.
	/// </summary>
	class ACID_EXPORT SmoothFloat
	{
	private:
		float m_agility;
		float m_target;
		float m_actual;
	public:
		/// <summary>
		/// Creates a new smooth float.
		/// </summary>
		/// <param name="initialValue"> The initial value. </param>
		/// <param name="agility"> The agility for increasing actual. </param>
		SmoothFloat(const float &initialValue, const float &agility);

		~SmoothFloat();

		/// <summary>
		/// Updates the driver with the passed time.
		/// </summary>
		/// <param name="delta"> The time between the last update. </param>
		void Update(const float &delta);

		/// <summary>
		/// Sets the target for the smooth float.
		/// </summary>
		/// <param name="target"> The new target. </param>
		void SetTarget(const float &target) { m_target = target; }

		/// <summary>
		/// Gets the smooth floats current target.
		/// </summary>
		/// <returns> The target. </returns>
		float GetTarget() const { return m_target; }

		/// <summary>
		/// Increases the smooth floats target.
		/// </summary>
		/// <param name="increase"> How much to increase the target by. </param>
		void IncreaseTarget(const float &increase) { m_target += increase; }

		/// <summary>
		/// Gets the currently calculated value.
		/// </summary>
		/// <returns> The calculated value. </returns>
		float Get() const { return m_actual; }
	};
}
