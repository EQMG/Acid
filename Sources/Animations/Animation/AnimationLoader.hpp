#pragma once

#include "Animation.hpp"

namespace acid
{
	class ACID_EXPORT AnimationLoader
	{
	private:
		Metadata *m_libraryAnimations;
		Metadata *m_libraryVisualScenes;

		Time m_lengthSeconds;
		std::vector<KeyframeData> m_keyframeData;
	public:
		AnimationLoader(Metadata *libraryAnimations, Metadata *libraryVisualScenes);

		Time GetLengthSeconds() const { return m_lengthSeconds; }

		std::vector<KeyframeData> GetKeyframeData() const { return m_keyframeData; }
	private:
		std::string FindRootJointName();

		std::vector<Time> GetKeyTimes();

		void CreateKeyframeData(const std::vector<Time> &times);

		void LoadJointTransforms(Metadata *jointData, const std::string &rootNodeId);

		std::string GetDataId(Metadata *jointData);

		std::string GetJointName(Metadata *jointData);

		void ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, const bool &root);
	};
}
