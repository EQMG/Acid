#pragma once

#include "Maths/Matrix4.hpp"
#include "Files/Node.hpp"
#include "Animation.hpp"

namespace acid {
class ACID_EXPORT AnimationLoader {
public:
	AnimationLoader(NodeConstView &&libraryAnimations, NodeConstView &&libraryVisualScenes, const Matrix4 &correction);

	const Time &GetLengthSeconds() const { return lengthSeconds; }
	const std::vector<Keyframe> &GetKeyframes() const { return keyframes; }

private:
	std::string FindRootJointName() const;
	std::vector<Time> GetKeyTimes() const;

	void CreateKeyframe(const std::vector<Time> &times);
	void LoadJointTransforms(const Node &jointData, const std::string &rootNodeId);

	static std::string GetDataId(const Node &jointData);
	static std::string GetJointName(const Node &jointData);

	void ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, bool root);

	NodeConstView libraryAnimations;
	NodeConstView libraryVisualScenes;
	Matrix4 correction;

	Time lengthSeconds;
	std::vector<Keyframe> keyframes;
};
}
