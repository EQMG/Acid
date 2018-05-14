#pragma once

#include "Maths/Maths.hpp"
#include "Maths/Matrix4.hpp"
#include "Meshes/Mesh.hpp"
#include "Animation/AnimationLoader.hpp"
#include "Geometry/GeometryLoader.hpp"
#include "Objects/Component.hpp"
#include "Skeleton/SkeletonLoader.hpp"
#include "Skin/SkinLoader.hpp"
#include "Animator.hpp"

namespace fl
{
	class FL_EXPORT MeshAnimated :
		public Mesh
	{
	private:
		std::string m_filename;

		Model *m_model;
		Joint *m_headJoint;
		Animation *m_animation;

		Animator *m_animator;
		std::vector<Matrix4 *> m_jointMatrices;
	public:
		static const Matrix4 *S_CORRECTION;
		static const int MAX_WEIGHTS;

		MeshAnimated(const std::string &filename = "");

		~MeshAnimated();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "MeshAnimated"; };

		Model *GetModel() const override { return m_model; }

		void SetModel(Model *model) override { m_model = model; }

		void TrySetModel(const std::string &filename) override;

		std::vector<Matrix4 *> GetJointTransforms() const { return m_jointMatrices; }

	private:
		Joint *CreateJoints(JointData *data);

		void AddJointsToArray(const Joint &headJoint, std::vector<Matrix4 *> *jointMatrices);
	};
}