#include "Animator.hpp"

#include <cmath>
#include "../../Engine/Engine.hpp"

namespace Flounder
{
	Animator::Animator(Joint *rootJoint) :
		m_rootJoint(rootJoint),
		m_animationTime(0.0f),
		m_currentAnimation(nullptr),
		m_animatorTransformation(new Matrix4())
	{
	}

	Animator::~Animator()
	{
		delete m_rootJoint;

		delete m_currentAnimation;
		delete m_animatorTransformation;
	}

	void Animator::Update()
	{

		if (m_currentAnimation == nullptr)
		{
			return;
		}

		IncreaseAnimationTime();
		auto currentPose = CalculateCurrentAnimationPose();
		ApplyPoseToJoints(currentPose, m_rootJoint, *m_animatorTransformation->SetIdentity());
	}

	void Animator::IncreaseAnimationTime()
	{
		m_animationTime += Engine::Get()->GetDelta();

		if (m_animationTime > m_currentAnimation->GetLength())
		{
			m_animationTime = std::fmod(m_animationTime, m_currentAnimation->GetLength());
		}
	}

	std::map<std::string, Matrix4 *> Animator::CalculateCurrentAnimationPose()
	{
		auto frames = GetPreviousAndNextFrames();
		float progression = CalculateProgression(*frames.at(0), *frames.at(1));
		return InterpolatePoses(*frames.at(0), *frames.at(1), progression);
	}

	std::vector<Keyframe *> Animator::GetPreviousAndNextFrames()
	{
		auto allFrames = m_currentAnimation->GetKeyframes();
		Keyframe *previousFrame = allFrames->at(0);
		Keyframe *nextFrame = allFrames->at(0);

		for (unsigned int i = 1; i < allFrames->size(); i++)
		{
			nextFrame = allFrames->at(i);

			if (nextFrame->GetTimeStamp() > m_animationTime)
			{
				break;
			}

			previousFrame = allFrames->at(i);
		}

		return { previousFrame, nextFrame };
	}

	float Animator::CalculateProgression(const Keyframe &previousFrame, const Keyframe &nextFrame)
	{
		float totalTime = nextFrame.GetTimeStamp() - previousFrame.GetTimeStamp();
		float currentTime = m_animationTime - previousFrame.GetTimeStamp();
		return currentTime / totalTime;
	}

	std::map<std::string, Matrix4 *> Animator::InterpolatePoses(const Keyframe &previousFrame, const Keyframe &nextFrame, const float &progression)
	{
		auto currentPose = std::map<std::string, Matrix4 *>();

		for (auto joint : *previousFrame.GetPose())
		{
			JointTransform *previousTransform = previousFrame.GetPose()->find(joint.first)->second;
			JointTransform *nextTransform = nextFrame.GetPose()->find(joint.first)->second;
			JointTransform *currentTransform = JointTransform::Interpolate(*previousTransform, *nextTransform, progression);
			currentPose.insert(std::make_pair(joint.first, currentTransform->GetLocalTransform()));
			delete currentTransform;
		}

		return currentPose;
	}

	void Animator::ApplyPoseToJoints(const std::map<std::string, Matrix4 *> &currentPose, Joint *joint, const Matrix4 &parentTransform)
	{
		Matrix4 *currentLocalTransform = currentPose.find(joint->GetName())->second;
		Matrix4 *currentTransform = Matrix4::Multiply(parentTransform, *currentLocalTransform, nullptr);

		for (auto childJoint : *joint->GetChildren())
		{
			ApplyPoseToJoints(currentPose, childJoint, *currentTransform);
		}

		Matrix4::Multiply(*currentTransform, *joint->GetInverseBindTransform(), currentTransform);
		joint->SetAnimatedTransform(*currentTransform);

		delete currentTransform;
	}

	void Animator::DoAnimation(Animation *animation)
	{
		m_animationTime = 0;
		m_currentAnimation = animation;
	}
}
