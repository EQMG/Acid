#pragma once

#include "Maths/Time.hpp"
#include "Keyframe.hpp"

namespace acid {
/**
 * @brief Class that represents an animation that can be carried out by an animated entity.
 * It contains the length of the animation in seconds, and a list of {@link Keyframe}s.
 **/
class ACID_EXPORT Animation {
public:
	/**
	 * Creates a new animation.
	 * @param length The length of the animation.
	 * @param keyframes All the keyframes for the animation, ordered by time of appearance in the animation.
	 **/
	Animation(const Time &length, std::vector<Keyframe> keyframes);

	/**
	 * Gets the length of the animation.
	 * @return The length of the animation.
	 **/
	const Time &GetLength() const { return m_length; }

	/**
	 * Gets an array of the animation's keyframes. The array is ordered based on the order of the keyframes in the animation.
	 * @return The array of the animation's keyframes.
	 **/
	const std::vector<Keyframe> &GetKeyframes() const { return m_keyframes; }

	friend const Node &operator>>(const Node &node, Animation &animation);
	friend Node &operator<<(Node &node, const Animation &animation);

private:
	Time m_length;
	std::vector<Keyframe> m_keyframes;
};
}
