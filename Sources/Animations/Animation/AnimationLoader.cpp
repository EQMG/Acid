#include "AnimationLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid
{
	AnimationLoader::AnimationLoader(LoadedValue *libraryAnimations, LoadedValue *libraryVisualScenes) :
		m_libraryAnimations(libraryAnimations),
		m_libraryVisualScenes(libraryVisualScenes)
	{
		auto animationNodes = m_libraryAnimations->GetChildren("animation");

		std::string rootNode = FindRootJointName();
		auto times = GetKeyTimes();
		m_lengthSeconds = times[times.size() - 1];
		CreateKeyframeData(times);

		for (auto &jointNode : animationNodes)
		{
			LoadJointTransforms(jointNode, rootNode);
		}
	}

	AnimationLoader::~AnimationLoader()
	{
		for (auto &keyframeData : m_keyframeData)
		{
			delete keyframeData;
		}
	}

	std::string AnimationLoader::FindRootJointName()
	{
		LoadedValue *skeleton = m_libraryVisualScenes->GetChild("visual_scene")->GetChildWithAttribute("node", "id", "Armature");
		return skeleton->GetChild("node")->GetAttribute("id");
	}

	std::vector<float> AnimationLoader::GetKeyTimes()
	{
		LoadedValue *timeData = m_libraryAnimations->GetChild("animation")->GetChild("source")->GetChild("float_array");
		auto rawTimes = FormatString::Split(timeData->GetValue(), " ");
		auto times = std::vector<float>(rawTimes.size());

		for (uint32_t i = 0; i < times.size(); i++)
		{
			times[i] = std::stof(rawTimes[i]);
		}

		return times;
	}

	void AnimationLoader::CreateKeyframeData(const std::vector<float> &times)
	{
		for (auto &time : times)
		{
			m_keyframeData.emplace_back(new KeyframeData(time));
		}
	}

	void AnimationLoader::LoadJointTransforms(LoadedValue *jointData, const std::string &rootNodeId)
	{
		std::string jointNameId = GetJointName(jointData);
		std::string dataId = GetDataId(jointData);

		LoadedValue *transformData = jointData->GetChildWithAttribute("source", "id", dataId);

		std::string data = transformData->GetChild("float_array")->GetValue();
		auto splitData = FormatString::Split(data, " ");
		ProcessTransforms(jointNameId, splitData, jointNameId == rootNodeId);
	}

	std::string AnimationLoader::GetDataId(LoadedValue *jointData)
	{
		LoadedValue *node = jointData->GetChild("sampler")->GetChildWithAttribute("input", "semantic", "OUTPUT");

		return node->GetAttribute("source").substr(1);
	}

	std::string AnimationLoader::GetJointName(LoadedValue *jointData)
	{
		auto channelNode = jointData->GetChild("channel");
		std::string data = channelNode->GetAttribute("target");
		auto splitData = FormatString::Split(data, "/");
		return splitData[0];
	}

	void AnimationLoader::ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, const bool &root)
	{
		for (uint32_t i = 0; i < m_keyframeData.size(); i++)
		{
			Matrix4 transform = Matrix4();

			for (uint32_t j = 0; j < 16; j++)
			{
				transform.m_linear[j] = std::stof(rawData[i * 16 + j]);
			}

			transform = transform.Transpose();

			if (root)
			{
				// Because in Blender z is up, but the engine is y up.
				transform = MeshAnimated::CORRECTION * transform;
			}

			m_keyframeData[i]->AddJointTransform(new JointTransformData(jointName, transform));
		}
	}
}
