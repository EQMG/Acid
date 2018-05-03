#pragma once

#include "Animation.hpp"

namespace fl
{
	class FL_EXPORT AnimationLoader
	{
	private:
		LoadedValue *m_libraryAnimations;
		LoadedValue *m_libraryVisualScenes;

		float m_lengthSeconds;
		std::vector<KeyframeData *> m_keyframeData;
	public:
		AnimationLoader(LoadedValue *libraryAnimations, LoadedValue *libraryVisualScenes);

		~AnimationLoader();

		float GetLengthSeconds() const { return m_lengthSeconds; }

		std::vector<KeyframeData *> GetKeyframeData() const { return m_keyframeData; }
	private:
		std::string FindRootJointName();

		std::vector<float> GetKeyTimes();

		void CreateKeyframeData(const std::vector<float> &times);

		void LoadJointTransforms(LoadedValue *jointData, const std::string &rootNodeId);

		std::string GetDataId(LoadedValue *jointData);

		std::string GetJointName(LoadedValue *jointData);

		void ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, const bool &root);
	};
}
