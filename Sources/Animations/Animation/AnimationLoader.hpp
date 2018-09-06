#pragma once

#include "Animation.hpp"

namespace acid
{
	class ACID_EXPORT AnimationLoader
	{
	private:
		std::shared_ptr<Metadata> m_libraryAnimations;
		std::shared_ptr<Metadata> m_libraryVisualScenes;

		float m_lengthSeconds;
		std::vector<KeyframeData> m_keyframeData;
	public:
		AnimationLoader(const std::shared_ptr<Metadata> &libraryAnimations, const std::shared_ptr<Metadata> &libraryVisualScenes);

		float GetLengthSeconds() const { return m_lengthSeconds; }

		std::vector<KeyframeData> GetKeyframeData() const { return m_keyframeData; }
	private:
		std::string FindRootJointName();

		std::vector<float> GetKeyTimes();

		void CreateKeyframeData(const std::vector<float> &times);

		void LoadJointTransforms(std::shared_ptr<Metadata> &jointData, const std::string &rootNodeId);

		std::string GetDataId(std::shared_ptr<Metadata> &jointData);

		std::string GetJointName(std::shared_ptr<Metadata> &jointData);

		void ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, const bool &root);
	};
}
