#pragma once

#include <vector>
#include "Maths/Time.hpp"
#include "Animations/Keyframe/Keyframe.hpp"

namespace acid
{
	/// <summary>
	/// Represents an animation that can be carried out by an animated entity.
	/// It contains the length of the animation in seconds, and a list of <seealso cref="Keyframe"/>s.
	/// </summary>
	class ACID_EXPORT Animation
	{
	public:
		/// <summary>
		/// Creates a new animation.
		/// </summary>
		/// <param name="length"> The length of the animation. </param>
		/// <param name="keyframes"> All the keyframes for the animation, ordered by time of appearance in the animation. </param>
		Animation(const Time &length, std::vector<Keyframe> keyframes);

		/// <summary>
		/// Gets the length of the animation.
		/// </summary>
		/// <returns> The length of the animation. </returns>
		const Time &GetLength() const { return m_length; }

		/// <summary>
		/// Gets an array of the animation's keyframes. The array is ordered based on the order of the keyframes in the animation.
		/// </summary>
		/// <returns> The array of the animation's keyframes. </returns>
		const std::vector<Keyframe> &GetKeyframes() const { return m_keyframes; }
	private:
		Time m_length;
		std::vector<Keyframe> m_keyframes;
	};
}
