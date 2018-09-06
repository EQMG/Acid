#pragma once

#include <vector>
#include "Serialized/Metadata.hpp"
#include "Animations/Keyframe/Keyframe.hpp"

namespace acid
{
	/// <summary>
	/// Represents an animation that can be carried out by an animated entity.
	/// It contains the length of the animation in seconds, and a list of <seealso cref="Keyframe"/>s.
	/// </summary>
	class ACID_EXPORT Animation
	{
	private:
		float m_length;
		std::vector<Keyframe> m_keyframes;
	public:
		/// <summary>
		/// Creates a new animation.
		/// </summary>
		/// <param name="length"> The length of the animation in seconds. </param>
		/// <param name="keyframes"> All the keyframes for the animation, ordered by time of appearance in the animation. </param>
		Animation(const float &length, const std::vector<Keyframe> &keyframes);

		/// <summary>
		/// Creates a new animation.
		/// </summary>
		/// <param name="length"> The length of the animation in seconds. </param>
		/// <param name="keyframeData"> All the keyframe data for the animation, ordered by time of appearance in the animation. </param>
		Animation(const float &length, const std::vector<KeyframeData> &keyframeData);

		/// <summary>
		/// Gets the length of the animation in seconds.
		/// </summary>
		/// <returns> The length of the animation. </returns>
		float GetLength() const { return m_length; }

		/// <summary>
		/// Gets an array of the animation's keyframes. The array is ordered based on the order of the keyframes in the animation.
		/// </summary>
		/// <returns> The array of the animation's keyframes. </returns>
		std::vector<Keyframe> GetKeyframes() const { return m_keyframes; }
	};
}
