#pragma once

#include "Maths/Time.hpp"
#include "Animations/Joint/JointTransform.hpp"

namespace acid
{
/**
 * @brief class that represents one keyframe of an animation. This contains the timestamp of the keyframe,
 * which is the time (in seconds) from the start of the animation when this keyframe occurs.
 *
 * It also contains the desired local-space transforms of all of the joints in the animated entity at this keyframe in the animation.
 * The joint transforms are stored in a map, indexed by the name of the joint that they should be applied to.
 **/
class ACID_EXPORT Keyframe
{
public:
	/**
	 * Creates a new keyframe at a timestamp.
	 * @param timeStamp The time that this keyframe occurs during the animation.
	 * @param pose The local-space transforms for all the joints at this keyframe, indexed by the name of the joint that they should be applied to.
	 **/
	Keyframe(const Time &timeStamp, std::map<std::string, JointTransform> pose);

	void AddJointTransform(const std::string &jointNameId, const Matrix4 &jointLocalTransform);

	/**
	 * Gets the time in seconds of the keyframe in the animation.
	 * @return The time in seconds.
	 **/
	const Time &GetTimeStamp() const { return m_timeStamp; }

	/**
	 * Gets the desired local-space transforms of all the joints at this keyframe, of the animation,
	 * indexed by the name of the joint that they correspond to.
	 * @return The desired local-space transforms.
	 **/
	const std::map<std::string, JointTransform> &GetPose() const { return m_pose; }

private:
	Time m_timeStamp;
	std::map<std::string, JointTransform> m_pose;
};
}
