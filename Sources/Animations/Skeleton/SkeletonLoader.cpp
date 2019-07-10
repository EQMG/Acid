#include "SkeletonLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid
{
SkeletonLoader::SkeletonLoader(const Node *libraryControllers, std::vector<std::string> boneOrder, const Matrix4 &correction) :
	m_boneOrder{std::move(boneOrder)},
	m_correction{correction}
{
	m_armatureData = libraryControllers->FindChild("visual_scene")->FindChildWithAttribute("node", "id", "Armature");
	auto headNode{m_armatureData->FindChild("node")};
	m_headJoint = LoadJointData(headNode, true);
	m_headJoint.CalculateInverseBindTransform(Matrix4{});
}

Joint SkeletonLoader::LoadJointData(const Node *jointNode, const bool &isRoot)
{
	auto joint{ExtractMainJointData(jointNode, isRoot)};

	for (const auto &childNode : jointNode->FindChildren("node"))
	{
		joint.AddChild(LoadJointData(childNode, false));
	}

	return joint;
}

Joint SkeletonLoader::ExtractMainJointData(const Node *jointNode, const bool &isRoot)
{
	auto nameId{*jointNode->FindAttribute("id")};
	auto index{GetBoneIndex(nameId)};
	auto matrixData{String::Split(jointNode->FindChild("matrix")->GetValue(), ' ')};

	assert(matrixData.size() == 16);

	Matrix4 transform;

	for (int32_t row{}; row < 4; row++)
	{
		for (int32_t col{}; col < 4; col++)
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
	return {*index, nameId, transform};
}

std::optional<uint32_t> SkeletonLoader::GetBoneIndex(const std::string &name)
{
	for (uint32_t i{}; i < m_boneOrder.size(); i++)
	{
		if (m_boneOrder[i] == name)
		{
			return i;
		}
	}

	return std::nullopt;
}
}
