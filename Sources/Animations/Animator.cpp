#include "Animator.hpp"

#include "Engine/Engine.hpp"

namespace acid
{
Animator::Animator(Joint *rootJoint) :
	m_rootJoint(rootJoint),
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
	ApplyPoseToJoints(currentPose, *m_rootJoint, Matrix4());
}

void Animator::IncreaseAnimationTime()
{
	m_animationTime += Engine::Get()->GetDelta();

	if (m_animationTime > m_currentAnimation->GetLength())
	{
		m_animationTime = Time::Seconds(std::fmod(m_animationTime.AsSeconds(), m_currentAnimation->GetLength().AsSeconds()));
	}
}

std::map<std::string, Matrix4> Animator::CalculateCurrentAnimationPose() const
{
	auto frames = GetPreviousAndNextFrames();
	auto progression = CalculateProgression(frames[0], frames[1]);
	return InterpolatePoses(frames[0], frames[1], progression);
}

std::array<Keyframe, 2> Animator::GetPreviousAndNextFrames() const
{
	auto allFrames = m_currentAnimation->GetKeyframes();
	auto previousFrame = allFrames[0];
	auto nextFrame = allFrames[0];

	for (uint32_t i = 1; i < allFrames.size(); i++)
	{
		nextFrame = allFrames[i];

		if (nextFrame.GetTimeStamp() > m_animationTime)
		{
			break;
		}

		previousFrame = allFrames[i];
	}

	return { previousFrame, nextFrame };
}

float Animator::CalculateProgression(const Keyframe &previousFrame, const Keyframe &nextFrame) const
{
	auto totalTime = nextFrame.GetTimeStamp() - previousFrame.GetTimeStamp();
	auto currentTime = m_animationTime - previousFrame.GetTimeStamp();
	return static_cast<float>(currentTime / totalTime);
}

std::map<std::string, Matrix4> Animator::InterpolatePoses(const Keyframe &previousFrame, const Keyframe &nextFrame, const float &progression) const
{
	auto currentPose = std::map<std::string, Matrix4>();

	for (const auto &[name, joint] : previousFrame.GetPose())
	{
		auto previousTransform = previousFrame.GetPose().find(name)->second;
		auto nextTransform = nextFrame.GetPose().find(name)->second;
		auto currentTransform = JointTransform::Interpolate(previousTransform, nextTransform, progression);
		currentPose.emplace(name, currentTransform.GetLocalTransform());
	}

	return currentPose;
}

void Animator::ApplyPoseToJoints(const std::map<std::string, Matrix4> &currentPose, Joint &joint, const Matrix4 &parentTransform)
{
	auto currentLocalTransform = currentPose.find(joint.GetName())->second;
	auto currentTransform = parentTransform * currentLocalTransform;

	for (const auto &childJoint : joint.GetChildren())
	{
		ApplyPoseToJoints(currentPose, *childJoint, currentTransform);
	}

	currentTransform = currentTransform * joint.GetInverseBindTransform();
	joint.SetAnimatedTransform(currentTransform);
}

void Animator::DoAnimation(Animation *animation)
{
	m_animationTime = 0s;
	m_currentAnimation = animation;
}
}
