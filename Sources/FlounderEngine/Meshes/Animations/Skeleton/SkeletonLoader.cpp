#include "SkeletonLoader.hpp"

#include "../MeshAnimated.hpp"

namespace Flounder
{

	SkeletonLoader::SkeletonLoader(LoadedValue *libraryControllers, const std::vector<std::string> &boneOrder) :
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

	JointData *SkeletonLoader::LoadJointData(LoadedValue *jointNode, const bool &isRoot)
	{
		JointData *joint = ExtractMainJointData(jointNode, isRoot);

		if (jointNode->GetChild("node"))
		{
			if (!jointNode->GetChild("node")->GetChild("-id"))
			{
				for (auto childNode : jointNode->GetChild("node")->m_children)
				{
					JointData *childJoint = LoadJointData(childNode, false);
					joint->AddChild(childJoint);
				}
			}
			else if (!jointNode->GetChild("node")->m_children.empty())
			{
				JointData *childJoint = LoadJointData(jointNode->GetChild("node"), false);
				joint->AddChild(childJoint);
			}
		}

		return joint;
	}

	JointData *SkeletonLoader::ExtractMainJointData(LoadedValue *jointNode, const bool &isRoot)
	{
		std::string nameId = jointNode->GetChild("-id")->GetString();
		auto index = GetBoneIndex(nameId);
		auto matrixData = FormatString::Split(jointNode->GetChild("matrix")->GetChild("#text")->GetString(), " ");
		Matrix4 matrix = ConvertData(matrixData);
		matrix.Transpose();

		if (isRoot)
		{
			// Because in Blender z is up, but in our game y is up.
			Matrix4::Multiply(*MeshAnimated::S_CORRECTION, matrix, &matrix);
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
		float *data = new float[16];

		for (unsigned int i = 0; i < rawData.size(); i++)
		{
			data[i] = std::stof(rawData[i]);
		}

		Matrix4 result = Matrix4(data);
		delete[] data;
		return result;
	}
}
