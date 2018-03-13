#include "Animation.hpp"

namespace Flounder
{
	Animation::Animation(const float &length, const std::vector<Keyframe *> &keyframes) :
		m_length(length),
		m_keyframes(new std::vector<Keyframe*>(keyframes)),
		m_libraryAnimations(nullptr),
		m_libraryVisualScenes(nullptr),
		m_animationData(nullptr)
	{
	}

	Animation::Animation(const AnimationData &data) :
		m_length(data.GetLengthSeconds()),
		m_keyframes(new std::vector<Keyframe*>()),
		m_libraryAnimations(nullptr),
		m_libraryVisualScenes(nullptr),
		m_animationData(nullptr)
	{
		for (auto frameData : *data.GetKeyframes())
		{
			m_keyframes->push_back(new Keyframe(*frameData));
		}
	}

	Animation::Animation(LoadedValue *libraryAnimations, LoadedValue *libraryVisualScenes) :
		m_length(0),
		m_keyframes(new std::vector<Keyframe*>()),
		m_libraryAnimations(libraryAnimations),
		m_libraryVisualScenes(libraryVisualScenes),
		m_animationData(nullptr)
	{
	//	std::string rootNode = FindRootJointName();
	//	std::vector<float> times = GetKeyTimes();
	//	float duration = times.at(times.size() - 1);
	//	std::vector<KeyframeData *> keyframes = InitKeyframes(times);
	//	auto animationNodes = m_libraryAnimations->GetChild("animation")->m_children;
//
	//	for (auto jointNode : animationNodes)
	//	{
	//		LoadJointTransforms(keyframes, jointNode, rootNode);
	//	}
//
	//	m_animationData = new AnimationData(duration, keyframes);
	}

	Animation::~Animation()
	{
		for (auto keyFrame : *m_keyframes)
		{
			delete keyFrame;
		}

		delete m_keyframes;

		delete m_animationData;
	}
}
