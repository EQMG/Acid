#include "VoxelBlock.hpp"

#include "VoxelChunk.hpp"

namespace test
{
	std::map<std::string, Colour> VoxelBlock::BLOCK_COLOURS = std::map<std::string, Colour>
	{
		{"", Colour("#FFFFFF", 0.0f)},
		{"Grass", Colour("#5E7831")},
		{"Dirt", Colour("#784800")},
		{"Stone", Colour("#8B8D7A")},
	};

	VoxelBlock::VoxelBlock(VoxelChunk *parent, const Vector3 &position, const std::string &type) :
		m_parent(parent),
		m_position(position),
		m_type(type)
	{
	}

	void VoxelBlock::SetChanged()
	{
		m_parent->Rebuild();
	}

	Colour VoxelBlock::FindColour(const std::string &key)
	{
		auto it = BLOCK_COLOURS.find(key);

		if (it == BLOCK_COLOURS.end())
		{
#if ACID_VERBOSE
			Log::Error("Could not find a Block Face colour from key: %s\n", key.c_str());
#endif
			auto colour = Colour();
			BLOCK_COLOURS.emplace(key, colour);
			return colour;
		}

		return it->second;
	}
}
