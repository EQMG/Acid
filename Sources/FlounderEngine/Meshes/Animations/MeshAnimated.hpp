#pragma once

#include "../../Maths/Maths.hpp"
#include "../../Maths/Matrix4.hpp"
#include "../../Objects/Component.hpp"
#include "Animation/AnimationLoader.hpp"
#include "Skeleton/SkeletonLoader.hpp"
#include "Geometry/GeometryLoader.hpp"
#include "Skin/SkinLoader.hpp"
#include "Animator.hpp"

namespace Flounder
{
	class F_EXPORT MeshAnimated :
		public Component
	{
	private:
		std::string m_filename;

		Model *m_model;
		Joint *m_headJoint;
		Animation *m_animation;

		Animator *m_animator;
	public:
		static const Matrix4 *S_CORRECTION;
		static const int MAX_WEIGHTS;

		MeshAnimated(const std::string &filename = "");

		~MeshAnimated();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "MeshAnimated"; };

		void TrySetModel(const std::string &filename);
	private:
		Joint *CreateJoints(JointData *data);
	};
}