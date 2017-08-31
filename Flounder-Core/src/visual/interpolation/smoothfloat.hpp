#pragma once

namespace Flounder
{
	/// <summary>
	/// A class that smoothly increases its value.
	/// </summary>
	class smoothfloat
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
		smoothfloat(const float &initialValue, const float &agility);

		/// <summary>
		/// Deconstructor for smooth float.
		/// </summary>
		~smoothfloat();

		/// <summary>
		/// Updates the driver with the passed time.
		/// </summary>
		/// <param name="delta"> The time between the last update. </param>
		void update(const float &delta);

		/// <summary>
		/// Sets the target for the smooth float.
		/// </summary>
		/// <param name="target"> The new target. </param>
		void setTarget(const float &target) { m_target = target; }

		/// <summary>
		/// Gets the smooth floats current target.
		/// </summary>
		/// <returns> The target. </returns>
		float getTarget() { return m_target; }

		/// <summary>
		/// Increases the smooth floats target.
		/// </summary>
		/// <param name="increase"> How much to increase the target by. </param>
		void increaseTarget(const float &increase) { m_target += increase; }

		/// <summary>
		/// Gets the currently calculated value.
		/// </summary>
		/// <returns> The calculated value. </returns>
		float get() { return m_actual; }
	};
}
