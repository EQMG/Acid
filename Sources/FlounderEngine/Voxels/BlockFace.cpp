#include "BlockFace.hpp"

namespace Flounder
{
	std::map<short, Colour*> BlockFace::s_colours = std::map<short, Colour*>
		{
			{0, new Colour("#FFFFFF", 0.0f)},
			{1, new Colour("#5E7831")}, // Grass
			{2, new Colour("#784800")}, // Dirt
			{3, new Colour("#8B8D7A")}, // Stone
		};

	BlockFace::BlockFace(const FaceSide &faceSide, const short &blockType) :
		m_faceSide(faceSide),
		m_blockType(blockType)
	{
	}

	Colour *BlockFace::FindColour(const short &key)
	{
		const auto it = s_colours.find(key);

		if (it == s_colours.end())
		{
#if FLOUNDER_VERBOSE
			printf("Could not find a Block Face colour from key: %i", key);
#endif
			auto colour = new Colour();
			s_colours.insert(std::make_pair(key, colour));
			return colour;
		}

		return it->second;
	}

	bool BlockFace::operator==(const BlockFace &other) const
	{
		return m_faceSide == other.m_faceSide && m_blockType == other.m_blockType;
	}

	bool BlockFace::operator!=(const BlockFace &other) const
	{
		return !(*this == other);
	}
}
