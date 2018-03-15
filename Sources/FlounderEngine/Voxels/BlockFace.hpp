#pragma once

#include "../Meshes/Mesh.hpp"
#include "Block.hpp"

namespace Flounder
{
	enum BlockFaceType
	{
		TypeFront = 0,
		TypeBack = 1,
		TypeTop = 2,
		TypeBottom = 3,
		TypeLeft = 4,
		TypeRight = 5
	};

	class BlockFace
	{
	private:
		BlockFaceType m_faceType;
		short m_blockType;
		Vector3 m_position;
		Vector3 m_scale;
	public:
		BlockFace(const BlockFaceType &faceType, const Vector3 &position, const short &blockType, const Vector3 &scale = Vector3::ONE);

		BlockFace(const std::vector<int> &x, const std::vector<int> &du, const std::vector<int> &dv, const short &blockType);

		BlockFaceType GetFaceType() const { return m_faceType; }

		void AppendVertices(std::vector<Vertex> *vertices) const;

		void AppendIndices(std::vector<uint32_t> *indices, unsigned int indexStart) const;

		static bool CompareFaces(const BlockFace &face1, const BlockFace &face2);

		bool operator==(const BlockFace &other) const;

		bool operator!=(const BlockFace &other) const;
	};
}
