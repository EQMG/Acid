#pragma once

#include "../Meshes/Mesh.hpp"
#include "Block.hpp"

namespace Flounder
{
	enum FaceSide
	{
		FaceFront = 0,
		FaceBack = 1,
		FaceTop = 2,
		FaceBottom = 3,
		FaceLeft = 4,
		FaceRight = 5
	};

	class BlockFace
	{
	private:
		static std::map<std::string, Colour*> s_colours;

		FaceSide m_faceSide;
		std::string m_blockType;
	public:
		BlockFace(const FaceSide &faceSide, const std::string &blockType);

		FaceSide GetFaceSide() const { return m_faceSide; }

		std::string GetBlockType() const { return m_blockType; }

		static Colour *FindColour(const std::string &key);

		bool operator==(const BlockFace &other) const;

		bool operator!=(const BlockFace &other) const;
	};
}
