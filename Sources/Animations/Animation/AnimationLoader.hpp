#pragma once

#include "Serialized/Metadata.hpp"
#include "Animation.hpp"

namespace acid
{
class ACID_EXPORT AnimationLoader
{
public:
	AnimationLoader(const Metadata *libraryAnimations, const Metadata *libraryVisualScenes);

	const Time &GetLengthSeconds() const { return m_lengthSeconds; }

	const std::vector<Keyframe> &GetKeyframes() const { return m_keyframes; }

private:
	std::string FindRootJointName() const;

	std::vector<Time> GetKeyTimes() const;

	void CreateKeyframe(const std::vector<Time> &times);

	void LoadJointTransforms(const Metadata *jointData, const std::string &rootNodeId);

	static std::string GetDataId(const Metadata *jointData);

	static std::string GetJointName(const Metadata *jointData);

	void ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, const bool &root);

	const Metadata *m_libraryAnimations;
	const Metadata *m_libraryVisualScenes;

	Time m_lengthSeconds;
	std::vector<Keyframe> m_keyframes;
};
}
