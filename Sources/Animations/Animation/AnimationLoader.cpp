#include "AnimationLoader.hpp"

#include "Animations/AnimatedMesh.hpp"
#include "Utils/Enumerate.hpp"

namespace acid {
AnimationLoader::AnimationLoader(NodeConstView &&libraryAnimations, NodeConstView &&libraryVisualScenes, const Matrix4 &correction) :
	libraryAnimations(std::move(libraryAnimations)),
	libraryVisualScenes(std::move(libraryVisualScenes)),
	correction(correction) {
	auto animationNodes = libraryAnimations["animation"];

	auto rootNode = FindRootJointName();
	auto times = GetKeyTimes();
	lengthSeconds = times[times.size() - 1];
	CreateKeyframe(times);

	for (auto &jointNode : animationNodes.GetProperties())
		LoadJointTransforms(jointNode, rootNode);
}

std::string AnimationLoader::FindRootJointName() const {
	auto skeleton = libraryVisualScenes["visual_scene"]["node"].GetPropertyWithValue("-id", "Armature");
	return skeleton["node"]["-id"].Get<std::string>();
}

std::vector<Time> AnimationLoader::GetKeyTimes() const {
	// Times should be the same for each pose so we grab the first joint times.
	auto timeData = libraryAnimations["animation"][0]["source"][0]["float_array"];
	auto rawTimes = String::Split(timeData.Get<std::string>(), ' ');

	std::vector<Time> times;
	times.reserve(rawTimes.size());
	for (const auto &rawTime : rawTimes)
		times.emplace_back(Time::Seconds(String::From<float>(rawTime)));
	return times;
}

void AnimationLoader::CreateKeyframe(const std::vector<Time> &times) {
	for (const auto &time : times)
		keyframes.emplace_back(Keyframe(time, {}));
}

void AnimationLoader::LoadJointTransforms(const Node &jointData, const std::string &rootNodeId) {
	auto jointNameId = GetJointName(jointData);
	auto dataId = GetDataId(jointData);

	auto transformData = jointData["source"].GetPropertyWithValue("-id", dataId);

	auto data = transformData["float_array"].Get<std::string>();
	auto splitData = String::Split(data, ' ');
	ProcessTransforms(jointNameId, splitData, jointNameId == rootNodeId);
}

std::string AnimationLoader::GetDataId(const Node &jointData) {
	auto node = jointData["sampler"]["input"].GetPropertyWithValue("-semantic", "OUTPUT");
	return node["-source"].Get<std::string>().substr(1);
}

std::string AnimationLoader::GetJointName(const Node &jointData) {
	auto channelNode = jointData["channel"];
	auto data = channelNode["-target"].Get<std::string>();
	auto splitData = String::Split(data, '/');
	return splitData[0];
}

void AnimationLoader::ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, bool root) {
	for (auto &&[i, keyframe] : Enumerate(keyframes)) {
		Matrix4 transform;

		for (uint32_t row = 0; row < 4; row++) {
			for (uint32_t col = 0; col < 4; col++) {
				transform[row][col] = String::From<float>(rawData[16 * i + (row * 4 + col)]);
			}
		}

		transform = transform.Transpose();
		if (root)
			transform = correction * transform;

		keyframe.AddJointTransform(jointName, transform);
	}
}
}
