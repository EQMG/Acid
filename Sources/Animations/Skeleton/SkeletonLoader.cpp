#include "SkeletonLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid
{
	SkeletonLoader::SkeletonLoader(LoadedValue *libraryControllers, const std::vector<std::string> &boneOrder) :
		m_armatureData(nullptr),
		m_boneOrder(boneOrder),
		m_headJoint(nullptr)
	{
		m_armatureData = libraryControllers->GetChild("visual_scene")->GetChildWithAttribute("node", "id", "Armature");
		auto headNode = m_armatureData->GetChild("node");
		m_headJoint = LoadJointData(headNode, true);
	}

	SkeletonLoader::~SkeletonLoader()
	{
		delete m_headJoint;
	}

	JointData *SkeletonLoader::LoadJointData(LoadedValue *jointNode, const bool &isRoot)
	{
		JointData *joint = ExtractMainJointData(jointNode, isRoot);

		for (auto &childNode : jointNode->GetChildren("node"))
		{
			joint->AddChild(LoadJointData(childNode, false));
		}

		return joint;
	}

	JointData *SkeletonLoader::ExtractMainJointData(LoadedValue *jointNode, const bool &isRoot)
	{
		std::string nameId = jointNode->GetAttribute("id");
		auto index = GetBoneIndex(nameId);
		auto matrixData = FormatString::Split(jointNode->GetChild("matrix")->GetValue(), " ");

		Matrix4 transform = Matrix4();

		for (uint32_t i = 0; i < matrixData.size(); i++)
		{
			transform.m_linear[i] = std::stof(matrixData[i]);
		}

		transform = transform.Transpose();

		if (isRoot)
		{
			// Because in Blender z is up, but the engine is y up.
			transform = MeshAnimated::CORRECTION * transform;
		}

		m_jointCount++;
		return new JointData(index, nameId, transform);
	}

	int SkeletonLoader::GetBoneIndex(const std::string &name)
	{
		for (uint32_t i = 0; i < m_boneOrder.size(); i++)
		{
			if (m_boneOrder[i] == name)
			{
				return i;
			}
		}

		return -1;
	}
}
