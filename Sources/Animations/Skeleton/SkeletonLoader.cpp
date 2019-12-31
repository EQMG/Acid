#include "SkeletonLoader.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Helpers/Enumerate.hpp"

namespace acid {
SkeletonLoader::SkeletonLoader(NodeView &&libraryControllers, std::vector<std::string> boneOrder, const Matrix4 &correction) :
	m_armatureData(libraryControllers["visual_scene"]["node"].GetPropertyWithValue("-id", "Armature")),
	m_boneOrder(std::move(boneOrder)),
	m_correction(correction) {
	auto headNode = m_armatureData["node"];
	m_headJoint = LoadJointData(headNode, true);
	m_headJoint.CalculateInverseBindTransform({});
}

Joint SkeletonLoader::LoadJointData(const Node &jointNode, bool isRoot) {
	auto joint = ExtractMainJointData(jointNode, isRoot);
	
	auto childJointNode = jointNode["node"];
	if (!childJointNode)
		return joint;
	
	if (childJointNode.GetType() == Node::Type::Array) {
		for (auto &childNode : childJointNode.GetProperties())
			joint.AddChild(LoadJointData(childNode, false));
	} else {
		joint.AddChild(LoadJointData(childJointNode, false));
	}
	
	return joint;
}

Joint SkeletonLoader::ExtractMainJointData(const Node &jointNode, bool isRoot) {
	auto nameId = jointNode["-id"].Get<std::string>();
	auto index = GetBoneIndex(nameId);
	auto matrixData = String::Split(jointNode["matrix"]["#text"].Get<std::string>(), ' ');

	assert(matrixData.size() == 16);

	Matrix4 transform;

	for (int32_t row = 0; row < 4; row++) {
		for (int32_t col = 0; col < 4; col++) {
			transform[row][col] = String::From<float>(matrixData[row * 4 + col]);
		}
	}

	transform = transform.Transpose();

	if (isRoot) {
		transform = m_correction * transform;
	}

	m_jointCount++;
	return {*index, nameId, transform};
}

std::optional<uint32_t> SkeletonLoader::GetBoneIndex(const std::string &name) const {
	for (const auto &[i, bone] : Enumerate(m_boneOrder)) {
		if (bone == name) {
			return i;
		}
	}

	return std::nullopt;
}
}
