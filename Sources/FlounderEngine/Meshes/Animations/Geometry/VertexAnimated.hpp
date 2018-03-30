#pragma once

#include <vector>
#include "../../../Maths/Vector2.hpp"
#include "../../../Maths/Vector3.hpp"
#include "../../../Engine/Platform.hpp"
#include "../../../Models/IVertex.hpp"

namespace Flounder
{
	class F_EXPORT VertexAnimated :
		public IVertex
	{
	public:
		Vector3 m_position;
		Vector2 m_uv;
		Vector3 m_normal;
		Vector3 m_tangent;
		Vector3 m_jointId;
		Vector3 m_vertexWeight;

		VertexAnimated(const Vector3 &position = Vector3(), const Vector2 &uv = Vector2(), const Vector3 &normal = Vector3(), const Vector3 &tangent = Vector3(), const Vector3 &jointId = Vector3(), const Vector3 &vertexWeight = Vector3());

		VertexAnimated(const VertexAnimated &source);

		~VertexAnimated();

		Vector3 GetPosition() const override { return m_position; };

		void SetPosition(const Vector3 &position) override { m_position = position; };

		size_t GetSize() const override { return sizeof(VertexAnimated); }

		void *GetData(std::vector<IVertex *> &vertices) override;

		static std::vector<VkVertexInputBindingDescription> GetInputDescription();
	};
}
