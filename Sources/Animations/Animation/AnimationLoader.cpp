#include "AnimationLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid
{
	AnimationLoader::AnimationLoader(Metadata *libraryAnimations, Metadata *libraryVisualScenes) :
		m_libraryAnimations(libraryAnimations),
		m_libraryVisualScenes(libraryVisualScenes),
		m_lengthSeconds(Time::ZERO),
		m_keyframes(std::vector<Keyframe>())
	{
		auto animationNodes = m_libraryAnimations->FindChildren("animation");

		std::string rootNode = FindRootJointName();
		auto times = GetKeyTimes();
		m_lengthSeconds = times[times.size() - 1];
		CreateKeyframe(times);

		for (auto &jointNode : animationNodes)
		{
			LoadJointTransforms(jointNode, rootNode);
		}
	}

	std::string AnimationLoader::FindRootJointName()
	{
		auto skeleton = m_libraryVisualScenes->FindChild("visual_scene")->FindChildWithAttribute("node", "id", "Armature");
		return skeleton->FindChild("node")->FindAttribute("id");
	}

	std::vector<Time> AnimationLoader::GetKeyTimes()
	{
		auto timeData = m_libraryAnimations->FindChild("animation")->FindChild("source")->FindChild("float_array");
		auto rawTimes = String::Split(timeData->GetValue(), " ");
		std::vector<Time> times = {};

		for (auto &rawTime : rawTimes)
		{
			times.emplace_back(Time::Seconds(String::From<float>(rawTime)));
		}

		return times;
	}

	void AnimationLoader::CreateKeyframe(const std::vector<Time> &times)
	{
		for (auto &time : times)
		{
			m_keyframes.emplace_back(Keyframe(time, {}));
		}
	}

	void AnimationLoader::LoadJointTransforms(Metadata *jointData, const std::string &rootNodeId)
	{
		std::string jointNameId = GetJointName(jointData);
		std::string dataId = GetDataId(jointData);

		auto transformData = jointData->FindChildWithAttribute("source", "id", dataId);

		std::string data = transformData->FindChild("float_array")->GetValue();
		auto splitData = String::Split(data, " ");
		ProcessTransforms(jointNameId, splitData, jointNameId == rootNodeId);
	}

	std::string AnimationLoader::GetDataId(Metadata *jointData)
	{
		auto node = jointData->FindChild("sampler")->FindChildWithAttribute("input", "semantic", "OUTPUT");
		return node->FindAttribute("source").substr(1);
	}

	std::string AnimationLoader::GetJointName(Metadata *jointData)
	{
		auto channelNode = jointData->FindChild("channel");
		std::string data = channelNode->FindAttribute("target");
		auto splitData = String::Split(data, "/");
		return splitData[0];
	}

	void AnimationLoader::ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, const bool &root)
	{
		for (uint32_t i = 0; i < m_keyframes.size(); i++)
		{
			Matrix4 transform = Matrix4();

			for (uint32_t j = 0; j < 16; j++)
			{
				transform.m_linear[j] = String::From<float>(rawData[i * 16 + j]);
			}

			transform = transform.Transpose();

			if (root)
			{
				// Because in Blender z is up, but the engine is y up.
				transform = MeshAnimated::CORRECTION * transform;
			}

			m_keyframes[i].AddJointTransform(jointName, transform);
		}
	}
}
