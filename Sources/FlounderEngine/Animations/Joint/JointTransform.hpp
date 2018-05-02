#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Quaternion.hpp"
#include "JointTransformData.hpp"

namespace fl
{
	/// <summary>
	/// Represents the local bone-space transform of a joint at a certain keyframe during an animation.
	/// This includes the position and rotation of the joint, relative to the parent joint (or relative to the model's origin if it's the root joint).
	/// The transform is stored as a position vector and a quaternion (rotation) so that these values can  be easily interpolated,
	/// a functionality that this class also provides.
	/// </summary>
	class FL_EXPORT JointTransform
	{
	private:
		Vector3 *m_position;
		Quaternion *m_rotation;

	public:
		/// <summary>
		/// Creates a new joint transformation.
		/// </summary>
		/// <param name="position"> The position of the joint relative to the parent joint (local-space) at a certain keyframe. </param>
		/// <param name="rotation"> The rotation of the joint relative to te parent joint (local-space) at a certain keyframe. </param>
		JointTransform(const Vector3 &position, const Quaternion &rotation);

		/// <summary>
		/// Creates a new joint transformation.
		/// </summary>
		/// <param name="localTransform"> The joint's local-transform at a certain keyframe of an animation. </param>
		JointTransform(const Matrix4 &localTransform);

		JointTransform(const JointTransformData &data);

		~JointTransform();

		/// <summary>
		/// In this method the local-space transform matrix is constructed by translating an identity matrix using the position variable and then applying the rotation.
		/// The rotation is applied by first converting the quaternion into a rotation matrix, which is then multiplied with the transform matrix.
		/// </summary>
		/// <returns> The local-space transform as a matrix. </returns>
		Matrix4 *GetLocalTransform();

		/// <summary>
		/// Interpolates between two transforms based on the progression value.
		/// The result is a new transform which is part way between the two original transforms.
		/// The translation can simply be linearly interpolated, but the rotation interpolation is slightly more complex,
		/// using a method called "SLERP" to spherically-linearly interpolate between 2 quaternions (rotations).
		/// This gives a much much better result than trying to linearly interpolate between Euler rotations.
		/// </summary>
		/// <param name="frameA"> The previous transform </param>
		/// <param name="frameB"> The next transform </param>
		/// <param name="progression"> A number between 0 and 1 indicating how far between the two transforms to interpolate.
		/// A progression value of 0 would return a transform equal to "frameA", a value of 1 would return a transform equal to "frameB".
		/// Everything else gives a transform somewhere in-between the two.
		/// </param>
		/// <returns> A new interpolated joint transformation. </returns>
		static JointTransform *Interpolate(const JointTransform &frameA, const JointTransform &frameB, const float &progression);

		/// <summary>
		/// Linearly interpolates between two translations based on a "progression" value.
		/// </summary>
		/// <param name="start"> The start translation. </param>
		/// <param name="end"> The end translation. </param>
		/// <param name="progression"> A value between 0 and 1 indicating how far to interpolate between the two translations.
		/// </param>
		/// <returns> The interpolated progressed vector. </returns>
		static Vector3 Interpolate(const Vector3 &start, const Vector3 &end, const float &progression);

		Vector3 *GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) { *m_position = position; }

		Quaternion *GetRotation() const { return m_rotation; }

		void SetRotation(const Quaternion &rotation) { *m_rotation = rotation; }
	};
}
