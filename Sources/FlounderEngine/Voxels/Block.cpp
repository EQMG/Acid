#include "Block.hpp"

#include "Chunk.hpp"

namespace Flounder
{
	std::map<std::string, Colour*> Block::s_colours = std::map<std::string, Colour*>
		{
			{"", new Colour("#FFFFFF", 0.0f)},
			{"Grass", new Colour("#5E7831")},
			{"Dirt", new Colour("#784800")},
			{"Stone", new Colour("#8B8D7A")},
		};

	Block::Block(Chunk *parent, const Vector3 &position, const std::string &type) :
		m_parent(parent),
		m_position(new Vector3(position)),
		m_type(type)
	{
	}

	Block::~Block()
	{
		delete m_position;
	}

	void Block::SetChanged()
	{
		m_parent->Rebuild();
	}

	Colour *Block::FindColour(const std::string &key)
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
}
