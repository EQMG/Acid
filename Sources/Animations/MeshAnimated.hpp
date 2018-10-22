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
	/// <summary>
	/// This class represents an animated armature with a skin mesh.
	/// </summary>
	class ACID_EXPORT MeshAnimated :
		public Mesh
	{
	private:
		std::string m_filename;

		std::shared_ptr<Model> m_model;
		std::unique_ptr<Joint> m_headJoint;
		std::unique_ptr<Animator> m_animator;
		std::unique_ptr<Animation> m_animation;

		std::vector<Matrix4> m_jointMatrices;
	public:
		static const Matrix4 CORRECTION;
		static const uint32_t MAX_JOINTS;
		static const uint32_t MAX_WEIGHTS;

		explicit MeshAnimated(const std::string &filename = "");

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		std::shared_ptr<Model> GetModel() const override { return m_model; }

		virtual VertexInput GetVertexInput(const uint32_t &binding = 0) const { return VertexAnimated::GetVertexInput(binding); }

		void SetModel(const std::shared_ptr<Model> &model) override { m_model = model; }

		void TrySetModel(const std::string &filename) override; // TODO: Remove

		std::vector<Matrix4> GetJointTransforms() const { return m_jointMatrices; }

	private:
		Joint *CreateJoints(const JointData &data);

		void AddJointsToArray(const Joint &headJoint, std::vector<Matrix4> &jointMatrices);
	};
}
