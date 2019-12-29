#include "Animator.hpp"

#include "Engine/Engine.hpp"
#include "Helpers/Enumerate.hpp"

namespace acid {
void Animator::Update(const Joint &rootJoint, std::vector<Matrix4> &jointMatrices) {
	if (!m_currentAnimation) {
		return;
	}

	IncreaseAnimationTime();
	auto currentPose = CalculateCurrentAnimationPose();
	CalculateJointPose(currentPose, rootJoint, {}, jointMatrices);
}

void Animator::IncreaseAnimationTime() {
	m_animationTime += Engine::Get()->GetDelta();

	if (m_animationTime > m_currentAnimation->GetLength()) {
		m_animationTime = Time::Seconds(std::fmod(m_animationTime.AsSeconds(), m_currentAnimation->GetLength().AsSeconds()));
	}
}

std::map<std::string, Matrix4> Animator::CalculateCurrentAnimationPose() const {
	auto [frame0, frame1] = GetPreviousAndNextFrames();
	auto progression = CalculateProgression(frame0, frame1);
	return InterpolatePoses(frame0, frame1, progression);
}

std::pair<Keyframe, Keyframe> Animator::GetPreviousAndNextFrames() const {
	const auto &allFrames = m_currentAnimation->GetKeyframes();
	const Keyframe *previousFrame = nullptr;
	const Keyframe *nextFrame = nullptr;

	for (const auto &[i, frame] : Enumerate(allFrames)) {
		nextFrame = &frame;
		if (frame.GetTimeStamp() > m_animationTime)
			break;

		previousFrame = &frame;
	}

	return {*previousFrame, *nextFrame};
}

float Animator::CalculateProgression(const Keyframe &previousFrame, const Keyframe &nextFrame) const {
	auto totalTime = nextFrame.GetTimeStamp() - previousFrame.GetTimeStamp();
	auto currentTime = m_animationTime - previousFrame.GetTimeStamp();
	return static_cast<float>(currentTime / totalTime);
}

std::map<std::string, Matrix4> Animator::InterpolatePoses(const Keyframe &previousFrame, const Keyframe &nextFrame, float progression) const {
	std::map<std::string, Matrix4> currentPose;

	for (const auto &[name, joint] : previousFrame.GetPose()) {
		auto previousTransform = previousFrame.GetPose().find(name)->second;
		auto nextTransform = nextFrame.GetPose().find(name)->second;
		auto currentTransform = JointTransform::Interpolate(previousTransform, nextTransform, progression);
		currentPose.emplace(name, currentTransform.GetLocalTransform());
	}

	return currentPose;
}

void Animator::CalculateJointPose(const std::map<std::string, Matrix4> &currentPose, const Joint &joint, const Matrix4 &parentTransform, std::vector<Matrix4> &jointMatrices) {
	auto currentLocalTransform = currentPose.find(joint.GetName())->second;
	auto currentTransform = parentTransform * currentLocalTransform;

	for (const auto &childJoint : joint.GetChildren()) {
		CalculateJointPose(currentPose, childJoint, currentTransform, jointMatrices);
	}

	currentTransform = currentTransform * joint.GetInverseBindTransform();

	if (joint.GetIndex() < jointMatrices.size()) {
		jointMatrices[joint.GetIndex()] = currentTransform;
	}
}

void Animator::DoAnimation(Animation *animation) {
	m_animationTime = 0s;
	m_currentAnimation = animation;
}
}
