#pragma once

#include "../Meshes/Mesh.hpp"
#include "Block.hpp"

namespace Flounder
{
	enum FaceType
	{
		FaceNull = 0,
		FaceFront = 1,
		FaceBack = 2,
		FaceTop = 3,
		FaceBottom = 4,
		FaceLeft = 5,
		FaceRight = 6
	};

	class BlockFace
	{
	private:
		FaceType m_faceType;
		short m_blockType;
	public:
		BlockFace(const FaceType &faceType, const short &blockType);

		FaceType GetFaceType() const { return m_faceType; }

		void AppendVertices(std::vector<Vertex> *vertices, const Vector3 &position, const Vector3 &scale) const;

		void AppendIndices(std::vector<uint32_t> *indices, unsigned int indexStart) const;

		bool operator==(const BlockFace &other) const;

		bool operator!=(const BlockFace &other) const;
	};
}
