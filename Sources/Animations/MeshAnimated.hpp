#pragma once

#include "Maths/Maths.hpp"
#include "Maths/Matrix4.hpp"
#include "Meshes/Mesh.hpp"
#include "Objects/IComponent.hpp"
#include "Animation/AnimationLoader.hpp"
#include "Geometry/GeometryLoader.hpp"
#include "Geometry/VertexAnimated.hpp"
#include "Skeleton/SkeletonLoader.hpp"
#include "Skin/SkinLoader.hpp"
#include "Animator.hpp"

namespace acid
{
	class ACID_EXPORT MeshAnimated :
		public Mesh
	{
	private:
		std::string m_filename;

		std::shared_ptr<Model> m_model;
		std::shared_ptr<Joint> m_headJoint;
		std::shared_ptr<Animator> m_animator;
		std::shared_ptr<Animation> m_animation;

		std::vector<Matrix4> m_jointMatrices;
	public:
		static const Matrix4 CORRECTION;
		static const int MAX_JOINTS;
		static const int MAX_WEIGHTS;

		MeshAnimated(const std::string &filename = "");

		~MeshAnimated();

		void Update() override;

		void Decode(const Node &node) override;

		void Encode(Node &node) const override;

		std::shared_ptr<Model> GetModel() const override { return m_model; }

		virtual VertexInput GetVertexInput() const { return VertexAnimated::GetVertexInput(); }

		void SetModel(const std::shared_ptr<Model> &model) override { m_model = model; }

		void TrySetModel(const std::string &filename) override;

		std::vector<Matrix4> GetJointTransforms() const { return m_jointMatrices; }

	private:
		std::shared_ptr<Joint> CreateJoints(JointData *data);

		void AddJointsToArray(const Joint &headJoint, std::vector<Matrix4> &jointMatrices);
	};
}
