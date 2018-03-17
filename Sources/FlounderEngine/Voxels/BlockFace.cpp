#include "BlockFace.hpp"

namespace Flounder
{
	std::map<std::string, Colour*> BlockFace::s_colours = std::map<std::string, Colour*>
		{
			{"", new Colour("#FFFFFF", 0.0f)},
			{"Grass", new Colour("#5E7831")},
			{"Dirt", new Colour("#784800")},
			{"Stone", new Colour("#8B8D7A")},
		};

	BlockFace::BlockFace(const FaceSide &faceSide, const std::string &blockType) :
		m_faceSide(faceSide),
		m_blockType(blockType)
	{
	}

	Colour *BlockFace::FindColour(const std::string &key)
	{
		const auto it = s_colours.find(key);

		if (it == s_colours.end())
		{
#if FLOUNDER_VERBOSE
			printf("Could not find a Block Face colour from key: %s", key.c_str());
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
