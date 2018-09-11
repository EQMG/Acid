#include "Animator.hpp"

#include <cmath>
#include "Engine/Engine.hpp"

namespace acid
{
	Animator::Animator(Joint *rootJoint) :
		m_rootJoint(rootJoint),
		m_animationTime(0.0f),
		m_currentAnimation(nullptr)
	{
	}

	void Animator::Update()
	{
		if (m_currentAnimation == nullptr)
		{
			return;
		}

		IncreaseAnimationTime();
		auto currentPose = CalculateCurrentAnimationPose();
		ApplyPoseToJoints(currentPose, *m_rootJoint, Matrix4::IDENTITY);
	}

	void Animator::IncreaseAnimationTime()
	{
		m_animationTime += Engine::Get()->GetDelta();

		if (m_animationTime > m_currentAnimation->GetLength())
		{
			m_animationTime = std::fmod(m_animationTime, m_currentAnimation->GetLength());
		}
	}

	std::map<std::string, Matrix4> Animator::CalculateCurrentAnimationPose()
	{
		auto frames = GetPreviousAndNextFrames();
		float progression = CalculateProgression(frames[0], frames[1]);
		return InterpolatePoses(frames[0], frames[1], progression);
	}

	std::array<Keyframe, 2> Animator::GetPreviousAndNextFrames()
	{
		auto allFrames = m_currentAnimation->GetKeyframes();
		Keyframe previousFrame = allFrames[0];
		Keyframe nextFrame = allFrames[0];

		for (uint32_t i = 1; i < allFrames.size(); i++)
		{
			nextFrame = allFrames[i];

			if (nextFrame.GetTimeStamp() > m_animationTime)
			{
				break;
			}

			previousFrame = allFrames[i];
		}

		return {previousFrame, nextFrame};
	}

	float Animator::CalculateProgression(const Keyframe &previousFrame, const Keyframe &nextFrame)
	{
		float totalTime = nextFrame.GetTimeStamp() - previousFrame.GetTimeStamp();
		float currentTime = m_animationTime - previousFrame.GetTimeStamp();
		return currentTime / totalTime;
	}

	std::map<std::string, Matrix4> Animator::InterpolatePoses(const Keyframe &previousFrame, const Keyframe &nextFrame, const float &progression)
	{
		auto currentPose = std::map<std::string, Matrix4>();

		for (auto &[name, joint] : previousFrame.GetPose())
		{
			JointTransform previousTransform = previousFrame.GetPose().find(name)->second;
			JointTransform nextTransform = nextFrame.GetPose().find(name)->second;
			JointTransform currentTransform = JointTransform::Interpolate(previousTransform, nextTransform, progression);
			currentPose.emplace(name, currentTransform.GetLocalTransform());
		}

		return currentPose;
	}

	void Animator::ApplyPoseToJoints(const std::map<std::string, Matrix4> &currentPose, Joint &joint, const Matrix4 &parentTransform)
	{
		Matrix4 currentLocalTransform = currentPose.find(joint.GetName())->second;
		Matrix4 currentTransform = parentTransform * currentLocalTransform;

		for (auto &childJoint : joint.GetChildren())
		{
			ApplyPoseToJoints(currentPose, *childJoint, currentTransform);
		}

		currentTransform = currentTransform * joint.GetInverseBindTransform();
		joint.SetAnimatedTransform(currentTransform);
	}

	void Animator::DoAnimation(Animation *animation)
	{
		m_animationTime = 0.0f;
		m_currentAnimation = animation;
	}
}
