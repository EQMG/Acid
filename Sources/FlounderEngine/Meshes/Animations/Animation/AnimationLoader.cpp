#include "AnimationLoader.hpp"

#include "../MeshAnimated.hpp"

namespace Flounder
{
	AnimationLoader::AnimationLoader(LoadedValue *libraryAnimations, LoadedValue *libraryVisualScenes) :
		m_libraryAnimations(libraryAnimations),
		m_libraryVisualScenes(libraryVisualScenes),
		m_animationData(nullptr)
	{
		std::string rootNode = FindRootJointName();
		std::vector<float> times = GetKeyTimes();
		float duration = times.at(times.size() - 1);
		std::vector<KeyframeData *> keyframes = InitKeyframes(times);
		auto animationNodes = m_libraryAnimations->GetChild("animation")->m_children;

		for (auto jointNode : animationNodes)
		{
			LoadJointTransforms(keyframes, jointNode, rootNode);
		}

		m_animationData = new AnimationData(duration, keyframes);
	}

	AnimationLoader::~AnimationLoader()
	{

		delete m_animationData;
	}

	std::string AnimationLoader::FindRootJointName()
	{
		LoadedValue *skeleton;

		for (auto child : m_libraryVisualScenes->GetChild("visual_scene")->GetChild("node")->m_children)
		{
			if (child->GetChild("-id")->m_value == "Armature")
			{
				skeleton = child;
				break;
			}
		}

		return skeleton->GetChild("node")->GetChild("-id")->m_value;
	}

	std::vector<float> AnimationLoader::GetKeyTimes()
	{
		return std::vector<float>();
	}

	std::vector<KeyframeData *> AnimationLoader::InitKeyframes(const std::vector<float> &times)
	{
		return std::vector<KeyframeData *>();
	}

	void AnimationLoader::LoadJointTransforms(const std::vector<KeyframeData *> &frames, LoadedValue *jointData, const std::string &rootNodeId)
	{
		std::string jointNameId = GetJointName(jointData);
		std::string dataId = GetDataId(jointData);

		LoadedValue *transformData;

		for (auto child : jointData->GetChild("source")->m_children)
		{
			if (child->GetChild("-id")->m_value == dataId)
			{
				transformData = child;
				break;
			}
		}

		std::string data = transformData->GetChild("float_array")->GetChild("#text")->m_value;
		auto splitData = FormatString::Split(data, " ");
		ProcessTransforms(jointNameId, splitData, frames, jointNameId == rootNodeId);
	}

	std::string AnimationLoader::GetDataId(LoadedValue *jointData)
	{
		LoadedValue *node;

		for (auto child : jointData->GetChild("sampler")->GetChild("input")->m_children)
		{
			if (child->GetChild("-semantic")->m_value == "OUTPUT")
			{
				node = child;
				break;
			}
		}

		std::string data = node->GetChild("-source")->m_value;
		return data.substr(1);
	}

	std::string AnimationLoader::GetJointName(LoadedValue *jointData)
	{
		auto channelNode = jointData->GetChild("channel");
		std::string data = channelNode->GetChild("-target")->m_value;
		std::vector<std::string> splitData = FormatString::Split(data, "/");
		return splitData.at(0);
	}

	void AnimationLoader::ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, const std::vector<KeyframeData *> &keyframes, const bool &root)
	{
		float *matrixData = new float[16];

		for (unsigned int i = 0; i < keyframes.size(); i++)
		{
			for (unsigned int j = 0; j < 16; j++)
			{
				matrixData[j] = FormatString::ConvertTo<float>(rawData.at(i * 16 + j));
			}

			Matrix4 transform = Matrix4(matrixData);
			transform.Transpose();

			if (root)
			{
				// Because up axis in Blender is different to up axis in game.
				Matrix4::Multiply(*MeshAnimated::S_CORRECTION, transform, &transform);
			}

			keyframes.at(i)->AddJointTransform(new JointTransformData(jointName, transform));
		}

		delete[] matrixData;
	}
}
