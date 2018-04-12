#include "AnimationLoader.hpp"

#include "../MeshAnimated.hpp"

namespace Flounder
{
	AnimationLoader::AnimationLoader(LoadedValue *libraryAnimations, LoadedValue *libraryVisualScenes) :
		m_libraryAnimations(libraryAnimations),
		m_libraryVisualScenes(libraryVisualScenes),
		m_lengthSeconds(0.0f),
		m_keyframeData(std::vector<KeyframeData *>())
	{
		auto animationNodes = m_libraryAnimations->GetChild("animation")->m_children;

		std::string rootNode = FindRootJointName();
		std::vector<float> times = GetKeyTimes();
		m_lengthSeconds = times[times.size() - 1];
		CreateKeyframeData(times);

		for (auto jointNode : *animationNodes)
		{
			LoadJointTransforms(jointNode, rootNode);
		}
	}

	AnimationLoader::~AnimationLoader()
	{
		for (auto keyframeData : m_keyframeData)
		{
			delete keyframeData;
		}
	}

	std::string AnimationLoader::FindRootJointName()
	{
		LoadedValue *skeleton = m_libraryVisualScenes->GetChild("visual_scene")->GetChildWithAttribute("node", "-id", "Armature");

		return skeleton->GetChild("node")->GetChild("-id")->GetString();
	}

	std::vector<float> AnimationLoader::GetKeyTimes()
	{
		LoadedValue *animationGroup = m_libraryAnimations->GetChild("animation")->GetChild(0); // TODO
		std::string sourceInput = animationGroup->GetChild("-id")->GetString() + "-input";
		LoadedValue *timeData = animationGroup->GetChildWithAttribute("source", "-id", sourceInput)->GetChild("float_array")->GetChild("#text");

		auto rawTimes = FormatString::Split(timeData->GetString(), " ");
		std::vector<float> times = std::vector<float>(rawTimes.size());

		for (unsigned int i = 0; i < times.size(); i++)
		{
			times[i] = std::stof(rawTimes[i]);
		}

		return times;
	}

	void AnimationLoader::CreateKeyframeData(const std::vector<float> &times)
	{
		for (unsigned int i = 0; i < times.size(); i++)
		{
			KeyframeData *keyframeData = new KeyframeData(times[i]);
			m_keyframeData.push_back(keyframeData);
		}
	}

	void AnimationLoader::LoadJointTransforms(LoadedValue *jointData, const std::string &rootNodeId)
	{
		std::string jointNameId = GetJointName(jointData);
		std::string dataId = GetDataId(jointData);

		LoadedValue *transformData = jointData->GetChildWithAttribute("source", "-id", dataId);

		std::string data = transformData->GetChild("float_array")->GetChild("#text")->GetString();
		auto splitData = FormatString::Split(data, " ");
		ProcessTransforms(jointNameId, splitData, jointNameId == rootNodeId);
	}

	std::string AnimationLoader::GetDataId(LoadedValue *jointData)
	{
		LoadedValue *node = jointData->GetChild("sampler")->GetChildWithAttribute("input", "-semantic", "OUTPUT");

		return node->GetChild("-source")->GetString().substr(1);
	}

	std::string AnimationLoader::GetJointName(LoadedValue *jointData)
	{
		auto channelNode = jointData->GetChild("channel");
		std::string data = channelNode->GetChild("-target")->GetString();
		std::vector<std::string> splitData = FormatString::Split(data, "/");
		return splitData[0];
	}

	void AnimationLoader::ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, const bool &root)
	{
		float *matrixData = new float[16];

		for (unsigned int i = 0; i < m_keyframeData.size(); i++)
		{
			for (unsigned int j = 0; j < 16; j++)
			{
				matrixData[j] = std::stof(rawData[i * 16 + j]);
			}

			Matrix4 transform = Matrix4(matrixData);
			transform.Transpose();

			if (root)
			{
				// Because up axis in Blender is different to up axis in game.
				Matrix4::Multiply(*MeshAnimated::S_CORRECTION, transform, &transform);
			}

			m_keyframeData[i]->AddJointTransform(new JointTransformData(jointName, transform));
		}

		delete[] matrixData;
	}
}
