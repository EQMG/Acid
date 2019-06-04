#include "SkeletonLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid
{
SkeletonLoader::SkeletonLoader(const Metadata *libraryControllers, std::vector<std::string> boneOrder, const Matrix4 &correction) :
	m_boneOrder{std::move(boneOrder)},
	m_correction{correction}
{
	m_armatureData = libraryControllers->FindChild("visual_scene")->FindChildWithAttribute("node", "id", "Armature");
	auto headNode = m_armatureData->FindChild("node");
	m_headJoint = LoadJointData(headNode, true);
}

std::unique_ptr<JointData> SkeletonLoader::LoadJointData(const Metadata *jointNode, const bool &isRoot)
{
	auto joint = ExtractMainJointData(jointNode, isRoot);

	for (const auto &childNode : jointNode->FindChildren("node"))
	{
		joint->AddChild(LoadJointData(childNode, false));
	}

	return joint;
}

std::unique_ptr<JointData> SkeletonLoader::ExtractMainJointData(const Metadata *jointNode, const bool &isRoot)
{
	auto nameId = *jointNode->FindAttribute("id");
	auto index = GetBoneIndex(nameId);
	auto matrixData = String::Split(jointNode->FindChild("matrix")->GetValue(), " ");

	assert(matrixData.size() == 16);

	Matrix4 transform;

	for (int32_t row = 0; row < 4; row++)
	{
		for (int32_t col = 0; col < 4; col++)
		{
			transform[row][col] = String::From<float>(matrixData[row * 4 + col]);
		}
	}
	
	transform = transform.Transpose();

	if (isRoot)
	{
		transform = m_correction * transform;
	}

	m_jointCount++;
	return std::make_unique<JointData>(*index, nameId, transform);
}

std::optional<uint32_t> SkeletonLoader::GetBoneIndex(const std::string &name)
{
	for (uint32_t i = 0; i < m_boneOrder.size(); i++)
	{
		if (m_boneOrder[i] == name)
		{
			return i;
		}
	}

	return std::nullopt;
}
}
