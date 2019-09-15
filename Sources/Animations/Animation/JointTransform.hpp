#pragma once

#include "Maths/Quaternion.hpp"

namespace acid {
/**
 * @brief Class that represents the local bone-space transform of a joint at a certain keyframe during an animation.
 * This includes the position and rotation of the joint, relative to the parent joint (or relative to the model's origin if it's the root joint).
 * The transform is stored as a position vector and a quaternion (rotation) so that these values can  be easily interpolated,
 * a functionality that this class also provides.
 **/
class ACID_EXPORT JointTransform {
public:
	/**
	 * Creates a new joint transformation.
	 **/
	JointTransform() = default;

	/**
	 * Creates a new joint transformation.
	 * @param position The position of the joint relative to the parent joint (local-space) at a certain keyframe.
	 * @param rotation The rotation of the joint relative to te parent joint (local-space) at a certain keyframe.
	 **/
	JointTransform(const Vector3f &position, const Quaternion &rotation);

	/**
	 * Creates a new joint transformation.
	 * @param localTransform The joint's local-transform at a certain keyframe of an animation.
	 **/
	explicit JointTransform(const Matrix4 &localTransform);

	/**
	 * In this method the local-space transform matrix is constructed by translating an identity matrix using the position variable and then applying the rotation.
	 * The rotation is applied by first converting the quaternion into a rotation matrix, which is then multiplied with the transform matrix.
	 * @return The local-space transform as a matrix.
	 **/
	Matrix4 GetLocalTransform() const;

	/**
	 * Interpolates between two transforms based on the progression value.
	 * The result is a new transform which is part way between the two original transforms.
	 * The translation can simply be linearly interpolated, but the rotation interpolation is slightly more complex,
	 * using a method called "SLERP" to spherically-linearly interpolate between 2 quaternions (rotations).
	 * This gives a much much better result than trying to linearly interpolate between Euler rotations.
	 * @param frameA The previous transform
	 * @param frameB The next transform
	 * @param progression A number between 0 and 1 indicating how far between the two transforms to interpolate.
	 * A progression value of 0 would return a transform equal to "frameA", a value of 1 would return a transform equal to "frameB".
	 * Everything else gives a transform somewhere in-between the two.
	 * @return A new interpolated joint transformation.
	 **/
	static JointTransform Interpolate(const JointTransform &frameA, const JointTransform &frameB, float progression);

	/**
	 * Linearly interpolates between two translations based on a "progression" value.
	 * @param start The start translation.
	 * @param end The end translation.
	 * @param progression A value between 0 and 1 indicating how far to interpolate between the two translations.
	 * @return The interpolated progressed vector.
	 **/
	static Vector3f Interpolate(const Vector3f &start, const Vector3f &end, float progression);

	const Vector3f &GetPosition() const { return m_position; }
	void SetPosition(const Vector3f &position) { m_position = position; }

	const Quaternion &GetRotation() const { return m_rotation; }
	void SetRotation(const Quaternion &rotation) { m_rotation = rotation; }

	friend const Node &operator>>(const Node &node, JointTransform &jointTransform);
	friend Node &operator<<(Node &node, const JointTransform &jointTransform);

private:
	Vector3f m_position;
	Quaternion m_rotation;
};
}
