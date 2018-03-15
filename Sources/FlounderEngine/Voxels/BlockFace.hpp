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
		Vector3 m_position;
		Vector3 m_scale;
		Colour *m_colour;
	public:
		BlockFace(const BlockFaceType &faceType, const Vector3 &position, Colour *colour);

		BlockFaceType GetFaceType() const { return m_faceType; }

		Colour *GetColour() const { return m_colour; }

		void AppendVertices(std::vector<Vertex> *vertices) const;

		void AppendIndices(std::vector<uint32_t> *indices, unsigned int indexStart) const;

		static bool CompareFaces(const BlockFace &face1, const BlockFace &face2);

		bool operator==(const BlockFace &other) const;

		bool operator!=(const BlockFace &other) const;
	};
}
