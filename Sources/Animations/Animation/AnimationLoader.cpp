#include "AnimationLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid {
AnimationLoader::AnimationLoader(NodeReturn &&libraryAnimations, NodeReturn &&libraryVisualScenes, const Matrix4 &correction) :
	m_libraryAnimations(std::move(libraryAnimations)),
	m_libraryVisualScenes(std::move(libraryVisualScenes)),
	m_correction(correction) {
	auto animationNodes = m_libraryAnimations["animation"];

	auto rootNode = FindRootJointName();
	auto times = GetKeyTimes();
	m_lengthSeconds = times[times.size() - 1];
	CreateKeyframe(times);

	for (auto &jointNode : animationNodes->GetProperties()) {
		LoadJointTransforms(jointNode, rootNode);
	}
}

std::string AnimationLoader::FindRootJointName() const {
	auto skeleton = m_libraryVisualScenes["visual_scene"]["node"]->GetPropertyWithValue("-id", "Armature");
	return skeleton["node"]["-id"]->GetValue();
}

std::vector<Time> AnimationLoader::GetKeyTimes() const {
	// Times should be the same for each pose so we grab the first joint times.
	auto timeData = m_libraryAnimations["animation"][0]["source"][0]["float_array"];
	auto rawTimes = String::Split(timeData["#text"]->GetValue(), ' ');

	std::vector<Time> times;
	times.reserve(rawTimes.size());

	for (const auto &rawTime : rawTimes) {
		times.emplace_back(Time::Seconds(String::From<float>(rawTime)));
	}

	return times;
}

void AnimationLoader::CreateKeyframe(const std::vector<Time> &times) {
	for (const auto &time : times) {
		m_keyframes.emplace_back(Keyframe(time, {}));
	}
}

void AnimationLoader::LoadJointTransforms(const Node &jointData, const std::string &rootNodeId) {
	auto jointNameId = GetJointName(jointData);
	auto dataId = GetDataId(jointData);

	auto transformData = jointData["source"]->GetPropertyWithValue("-id", dataId);

	auto data = transformData["float_array"]["#text"]->GetValue();
	auto splitData = String::Split(data, ' ');
	ProcessTransforms(jointNameId, splitData, jointNameId == rootNodeId);
}

std::string AnimationLoader::GetDataId(const Node &jointData) {
	auto node = jointData["sampler"]["input"]->GetPropertyWithValue("-semantic", "OUTPUT");
	return node["-source"]->GetValue().substr(1);
}

std::string AnimationLoader::GetJointName(const Node &jointData) {
	auto channelNode = jointData["channel"];
	auto data = channelNode["-target"]->GetValue();
	auto splitData = String::Split(data, '/');
	return splitData[0];
}

void AnimationLoader::ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, bool root) {
	for (uint32_t i = 0; i < m_keyframes.size(); i++) {
		Matrix4 transform;

		for (uint32_t row = 0; row < 4; row++) {
			for (uint32_t col = 0; col < 4; col++) {
				transform[row][col] = String::From<float>(rawData[16 * i + (row * 4 + col)]);
			}
		}

		transform = transform.Transpose();

		if (root) {
			transform = m_correction * transform;
		}

		m_keyframes[i].AddJointTransform(jointName, transform);
	}
}
}
