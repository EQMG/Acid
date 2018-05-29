#pragma once

#include "Joint/Joint.hpp"
#include "Animation/Animation.hpp"

namespace fl
{
	/// <summary>
	/// This class contains all the functionality to apply an animation to an animated entity.
	/// An Animator instance is associated with just one animated entity.
	/// It also keeps track of the running time (in seconds) of the current animation,
	/// along with a reference to the currently playing animation for the corresponding entity.
	/// <para>
	/// An Animator instance needs to be updated every frame, in order for it to keep updating the animation pose of the associated entity.
	/// The currently playing animation can be changed at any time using the doAnimation() method.
	/// The Animator will keep looping the current animation until a new animation is chosen.
	/// </para>
	/// <para>
	/// The Animator calculates the desired current animation pose by interpolating between the previous and next keyframes of the animation
	/// (based on the current animation time). The Animator then updates the transforms all of the joints each frame to match the current desired animation pose.
	/// </para>
	/// </summary>
	class FL_EXPORT Animator
	{
	private:
		Joint *m_rootJoint;

		float m_animationTime;
		Animation *m_currentAnimation;
	public:
		/// <summary>
		/// Creates a new animator.
		/// </summary>
		/// <param name="rootJoint"> The root joint of the joint hierarchy which makes up the "skeleton" of the entity. </param>
		Animator(Joint *rootJoint);

		~Animator();

		/// <summary>
		/// This method should be called each frame to update the animation currently being played. This increases the animation time (and loops it back to zero if necessary),
		/// finds the pose that the entity should be in at that time of the animation, and then applied that pose to all the entity's joints.
		/// </summary>
		void Update();

		/// <summary>
		/// Increases the current animation time which allows the animation to progress. If the current animation has reached the end then the timer is reset, causing the animation to loop.
		/// </summary>
		void IncreaseAnimationTime();

		/// <summary>
		/// This method returns the current animation pose of the entity. It returns
		/// the desired local-space transforms for all the joints in a map, indexed
		/// by the name of the joint that they correspond to.
		/// <para>
		/// The pose is calculated based on the previous and next keyframes in the
		/// current animation. Each keyframe provides the desired pose at a certain
		/// time in the animation, so the animated pose for the current time can be
		/// calculated by interpolating between the previous and next keyframe.
		/// </para>
		/// <para>
		/// This method first finds the preious and next keyframe, calculates how far
		/// between the two the current animation is, and then calculated the pose
		/// for the current animation time by interpolating between the transforms at
		/// those keyframes.
		/// </para>
		/// </summary>
		/// <returns> The current pose as a map of the desired local-space transforms for all the joints.
		/// The transforms are indexed by the name ID of the joint that they should be applied to. </returns>
		std::map<std::string, Matrix4> CalculateCurrentAnimationPose();

		/// <summary>
		/// Finds the previous keyframe in the animation and the next keyframe in the animation, and returns them in an array of length 2.
		/// If there is no  previous frame (perhaps current animation time is 0.5 and the first keyframe is at time 1.5)
		/// then the next keyframe is used as both the previous and next keyframe. The reverse happens if there is no next keyframe.
		/// </summary>
		/// <returns> The previous and next keyframes, in an array which therefore will always have a length of 2. </returns>
		std::vector<Keyframe *> GetPreviousAndNextFrames();

		/// <summary>
		/// Calculates how far between the previous and next keyframe the current animation time is, and returns it as a value between 0 and 1.
		/// </summary>
		/// <param name="previousFrame"> The previous keyframe in the animation. </param>
		/// <param name="nextFrame"> The next keyframe in the animation.
		/// </param>
		/// <returns> A number between 0 and 1 indicating how far between the two keyframes the current animation time is. </returns>
		float CalculateProgression(const Keyframe &previousFrame, const Keyframe &nextFrame);

		/// <summary>
		/// Calculates all the local-space joint transforms for the desired current pose by interpolating between
		/// the transforms at the previous and next keyframes.
		/// </summary>
		/// <param name="previousFrame"> The previous keyframe in the animation. </param>
		/// <param name="nextFrame"> The next keyframe in the animation. </param>
		/// <param name="progression"> A number between 0 and 1 indicating how far between the previous and next keyframes the current animation time is.
		/// </param>
		/// <returns> The local-space transforms for all the joints for the desired current pose.
		/// They are returned in a map, indexed by the name of the joint to which they should be applied. </returns>
		std::map<std::string, Matrix4> InterpolatePoses(const Keyframe &previousFrame, const Keyframe &nextFrame, const float &progression);

		/// <summary>
		/// This method applies the current pose to a given joint, and all of its descendants.
		/// It does this by getting the desired local-transform for the
		/// current joint, before applying it to the joint. Before applying the
		/// transformations it needs to be converted from local-space to model-space
		/// (so that they are relative to the model's origin, rather than relative to
		/// the parent joint). This can be done by multiplying the local-transform of
		/// the joint with the model-space transform of the parent joint.
		/// <para>
		/// The same thing is then done to all the child joints.
		/// </para>
		/// <para>
		/// Finally the inverse of the joint's bind transform is multiplied with the
		/// model-space transform of the joint. This basically "subtracts" the
		/// joint's original bind (no animation applied) transform from the desired
		/// pose transform. The result of this is then the transform required to move
		/// the joint from its original model-space transform to it's desired
		/// model-space posed transform. This is the transform that needs to be
		/// loaded up to the vertex shader and used to transform the vertices into
		/// the current pose.
		/// </para>
		/// </summary>
		/// <param name="currentPose"> A map of the local-space transforms for all the joints for the desired pose. The map is indexed by the name of the joint which the transform corresponds to. </param>
		/// <param name="joint"> The current joint which the pose should be applied to. </param>
		/// <param name="parentTransform"> The desired model-space transform of the parent joint for the pose. </param>
		void ApplyPoseToJoints(const std::map<std::string, Matrix4> &currentPose, Joint *joint, const Matrix4 &parentTransform);

		Animation *GetCurrentAnimation() const { return m_currentAnimation; }

		/// <summary>
		/// Indicates that the entity should carry out the given animation. Resets the animation time so that the new animation starts from the beginning.
		/// </summary>
		/// <param name="animation"> The new animation to carry out. </param>
		void DoAnimation(Animation *animation);
	};
}
