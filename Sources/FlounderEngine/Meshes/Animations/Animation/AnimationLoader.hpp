#pragma once

#include "Animation.hpp"

namespace Flounder
{
	class F_EXPORT AnimationLoader
	{
	private:

		LoadedValue *m_libraryAnimations;
		LoadedValue *m_libraryVisualScenes;

		AnimationData *m_animationData;
	public:
		AnimationLoader(LoadedValue *libraryAnimations, LoadedValue *libraryVisualScenes);

		~AnimationLoader();

		AnimationData *GetData() const { return m_animationData; }
	private:
		std::string FindRootJointName();

		std::vector<float> GetKeyTimes();

		std::vector<KeyframeData*> InitKeyframes(const std::vector<float> &times);

		void LoadJointTransforms(const std::vector<KeyframeData*> &frames, LoadedValue *jointData, const std::string &rootNodeId);

		std::string GetDataId(LoadedValue *jointData);

		std::string GetJointName(LoadedValue *jointData);

		void ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, const std::vector<KeyframeData*> &keyframes, const bool &root);
	};
}
