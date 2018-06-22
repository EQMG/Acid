#include "SkeletonLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace fl
{
	SkeletonLoader::SkeletonLoader(std::shared_ptr<LoadedValue> libraryControllers, const std::vector<std::string> &boneOrder) :
		m_armatureData(nullptr),
		m_boneOrder(boneOrder),
		m_jointCount(0),
		m_headJoint(nullptr)
	{
		m_armatureData = libraryControllers->GetChild("visual_scene")->GetChildWithAttribute("node", "-id", "Armature");
		auto headNode = m_armatureData->GetChild("node");
		m_headJoint = LoadJointData(headNode, true);
	}

	SkeletonLoader::~SkeletonLoader()
	{
		delete m_headJoint;
	}

	JointData *SkeletonLoader::LoadJointData(std::shared_ptr<LoadedValue> jointNode, const bool &isRoot)
	{
		JointData *joint = ExtractMainJointData(jointNode, isRoot);

		if (jointNode->GetChild("node"))
		{
			if (!jointNode->GetChild("node")->GetChild("-id"))
			{
				for (auto &childNode : jointNode->GetChild("node")->GetChildren())
				{
					joint->AddChild(LoadJointData(childNode, false));
				}
			}
			else if (!jointNode->GetChild("node")->GetChildren().empty())
			{
				joint->AddChild(LoadJointData(jointNode->GetChild("node"), false));
			}
		}

		return joint;
	}

	JointData *SkeletonLoader::ExtractMainJointData(std::shared_ptr<LoadedValue> jointNode, const bool &isRoot)
	{
		std::string nameId = jointNode->GetChild("-id")->GetString();
		auto index = GetBoneIndex(nameId);
		auto matrixData = FormatString::Split(jointNode->GetChild("matrix")->GetChild("#text")->GetString(), " ");
		Matrix4 matrix = ConvertData(matrixData).Transpose();

		if (isRoot)
		{
			// Because in Blender z is up, but the engine is y up.
			matrix *= MeshAnimated::CORRECTION;
		}

		m_jointCount++;
		return new JointData(index, nameId, matrix);
	}

	int SkeletonLoader::GetBoneIndex(const std::string &name)
	{
		for (unsigned int i = 0; i < m_boneOrder.size(); i++)
		{
			if (m_boneOrder[i] == name)
			{
				return i;
			}
		}

		return -1;
	}

	Matrix4 SkeletonLoader::ConvertData(const std::vector<std::string> &rawData)
	{
		Matrix4 result = Matrix4();

		for (unsigned int i = 0; i < rawData.size(); i++)
		{
			result.m_linear[i] = std::stof(rawData[i]);
		}

		return result;
	}
}
