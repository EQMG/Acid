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
		LoadedValue *skeleton = m_libraryVisualScenes->GetChild("visual_scene")->GetChildWithAttribute("node", "-id", "Armature");

		return skeleton->GetChild("node")->GetChild("-id")->GetString();
	}

	std::vector<float> AnimationLoader::GetKeyTimes()
	{
		//LoadedValue *timeData = m_libraryAnimations->GetChildWithAttribute("animation", "-id", "Armature_Torso_pose_matrix")->GetChild("float_array")->GetChild("#text");
		std::string testingTime = "1 0 0 0 0 -0.06466547 -0.997907 0 0 0.997907 -0.06466556 3.810999 0 0 0 1 1 0 0 0 0 -0.06466547 -0.997907 0 0 0.997907 -0.06466556 3.210999 0 0 0 1 1 0 0 0 0 -0.06466547 -0.997907 0 0 0.997907 -0.06466556 3.810999 0 0 0 1 1 0 0 0 0 -0.06466547 -0.997907 0 0 0.997907 -0.06466556 3.210999 0 0 0 1 1 0 0 0 0 -0.06466547 -0.997907 0 0 0.997907 -0.06466556 3.810999 0 0 0 1";
		auto rawTimes = FormatString::Split(testingTime, " "); // timeData->GetString()
		std::vector<float> times = std::vector<float>(rawTimes.size());

		for (int i = 0; i < times.size(); i++)
		{
			times[i] = FormatString::ConvertTo<float>(rawTimes[i]);
		}

		return times;
	}

	std::vector<KeyframeData *> AnimationLoader::InitKeyframes(const std::vector<float> &times)
	{
		std::vector<KeyframeData *> frames = std::vector<KeyframeData *>(times.size());

		for (int i = 0; i < frames.size(); i++) {

			KeyframeData keyframeData = KeyframeData(times[i]);
			frames[i] = &keyframeData;
		}

		return frames;
	}

	void AnimationLoader::LoadJointTransforms(const std::vector<KeyframeData *> &frames, LoadedValue *jointData, const std::string &rootNodeId)
	{
		std::string jointNameId = GetJointName(jointData);
		std::string dataId = GetDataId(jointData);

		LoadedValue *transformData = jointData->GetChildWithAttribute("source", "-id", dataId);

		std::string data = transformData->GetChild("float_array")->GetChild("#text")->GetString();
		auto splitData = FormatString::Split(data, " ");
		ProcessTransforms(jointNameId, splitData, frames, jointNameId == rootNodeId);
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
