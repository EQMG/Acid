#pragma once

#include "Joint/Joint.hpp"
#include "Animation/Animation.hpp"

namespace Flounder
{
	class F_EXPORT Animator
	{
	private:
		Joint *m_rootJoint;

		float m_animationTime;
		Animation *m_currentAnimation;
		Matrix4 *m_animatorTransformation;

	public:
		Animator(Joint *rootJoint);

		~Animator();

		void Update();

		void IncreaseAnimationTime();

		std::map<std::string, Matrix4*> CalculateCurrentAnimationPose();

		std::vector<Keyframe*> GetPreviousAndNextFrames();

		float CalculateProgression(const Keyframe &previousFrame, const Keyframe &nextFrame);

		std::map<std::string, Matrix4*> InterpolatePoses(const Keyframe &previousFrame, const Keyframe &nextFrame, const float &progression);

		void ApplyPoseToJoints(const std::map<std::string, Matrix4*> &currentPose, Joint *joint, const Matrix4 &parentTransform);

		Animation *GetCurrentAnimation() const { return m_currentAnimation; }

		void DoAnimation(Animation *animation);
	};
}
