#pragma once

#include "Maths/Matrix4.hpp"
#include "Files/Node.hpp"
#include "Animation.hpp"

namespace acid {
class ACID_EXPORT AnimationLoader {
public:
	AnimationLoader(NodeView &&libraryAnimations, NodeView &&libraryVisualScenes, const Matrix4 &correction);

	const Time &GetLengthSeconds() const { return m_lengthSeconds; }
	const std::vector<Keyframe> &GetKeyframes() const { return m_keyframes; }

private:
	std::string FindRootJointName() const;
	std::vector<Time> GetKeyTimes() const;

	void CreateKeyframe(const std::vector<Time> &times);
	void LoadJointTransforms(const Node &jointData, const std::string &rootNodeId);

	static std::string GetDataId(const Node &jointData);
	static std::string GetJointName(const Node &jointData);

	void ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, bool root);

	mutable NodeView m_libraryAnimations;
	mutable NodeView m_libraryVisualScenes;
	Matrix4 m_correction;

	Time m_lengthSeconds;
	std::vector<Keyframe> m_keyframes;
};
}
