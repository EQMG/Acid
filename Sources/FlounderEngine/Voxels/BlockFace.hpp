#pragma once

#include "../Meshes/Mesh.hpp"
#include "Block.hpp"

namespace Flounder
{
	enum BlockFaceType
	{
		TypeNull = 0,
		TypeFront = 1,
		TypeBack = 2,
		TypeTop = 3,
		TypeBottom = 4,
		TypeLeft = 5,
		TypeRight = 6
	};

	class BlockFace
	{
	private:
		BlockFaceType m_faceType;
		short m_blockType;
		Vector3 m_position;
		Vector3 m_scale;
	public:
		BlockFace(const BlockFaceType &faceType, const Vector3 &position, const Vector3 &scale, const short &blockType);

		BlockFaceType GetFaceType() const { return m_faceType; }

		void AppendVertices(std::vector<Vertex> *vertices) const;

		void AppendIndices(std::vector<uint32_t> *indices, unsigned int indexStart) const;

		static bool CompareFaces(const BlockFace &face1, const BlockFace &face2);

		bool operator==(const BlockFace &other) const;

		bool operator!=(const BlockFace &other) const;
	};
}
