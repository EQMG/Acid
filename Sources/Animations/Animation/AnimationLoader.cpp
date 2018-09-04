#include "AnimationLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid
{
	AnimationLoader::AnimationLoader(const std::shared_ptr<Node> &libraryAnimations, const std::shared_ptr<Node> &libraryVisualScenes) :
		m_libraryAnimations(libraryAnimations),
		m_libraryVisualScenes(libraryVisualScenes),
		m_lengthSeconds(0.0f),
		m_keyframeData(std::vector<KeyframeData>())
	{
		auto animationNodes = m_libraryAnimations->FindChildren("animation");

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
	}

	std::string AnimationLoader::FindRootJointName()
	{
		auto skeleton = m_libraryVisualScenes->FindChild("visual_scene")->FindChildWithAttribute("node", "id", "Armature");
		return skeleton->FindChild("node")->FindAttribute("id");
	}

	std::vector<float> AnimationLoader::GetKeyTimes()
	{
		auto timeData = m_libraryAnimations->FindChild("animation")->FindChild("source")->FindChild("float_array");
		auto rawTimes = String::Split(timeData->GetValue(), " ");
		auto times = std::vector<float>(rawTimes.size());

		for (uint32_t i = 0; i < times.size(); i++)
		{
			times[i] = String::FromString<float>(rawTimes[i]);
		}

		return times;
	}

	void AnimationLoader::CreateKeyframeData(const std::vector<float> &times)
	{
		for (auto &time : times)
		{
			m_keyframeData.emplace_back(KeyframeData(time));
		}
	}

	void AnimationLoader::LoadJointTransforms(std::shared_ptr<Node> &jointData, const std::string &rootNodeId)
	{
		std::string jointNameId = GetJointName(jointData);
		std::string dataId = GetDataId(jointData);

		auto transformData = jointData->FindChildWithAttribute("source", "id", dataId);

		std::string data = transformData->FindChild("float_array")->GetValue();
		auto splitData = String::Split(data, " ");
		ProcessTransforms(jointNameId, splitData, jointNameId == rootNodeId);
	}

	std::string AnimationLoader::GetDataId(std::shared_ptr<Node> &jointData)
	{
		auto node = jointData->FindChild("sampler")->FindChildWithAttribute("input", "semantic", "OUTPUT");
		return node->FindAttribute("source").substr(1);
	}

	std::string AnimationLoader::GetJointName(std::shared_ptr<Node> &jointData)
	{
		auto channelNode = jointData->FindChild("channel");
		std::string data = channelNode->FindAttribute("target");
		auto splitData = String::Split(data, "/");
		return splitData[0];
	}

	void AnimationLoader::ProcessTransforms(const std::string &jointName, const std::vector<std::string> &rawData, const bool &root)
	{
		for (uint32_t i = 0; i < m_keyframeData.size(); i++)
		{
			Matrix4 transform = Matrix4();

			for (uint32_t j = 0; j < 16; j++)
			{
				transform.m_linear[j] = String::FromString<float>(rawData[i * 16 + j]);
			}

			transform = transform.Transpose();

			if (root)
			{
				// Because in Blender z is up, but the engine is y up.
				transform = MeshAnimated::CORRECTION * transform;
			}

			m_keyframeData[i].AddJointTransform(JointTransformData(jointName, transform));
		}
	}
}
