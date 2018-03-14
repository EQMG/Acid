#include "Block.hpp"

#include "Chunk.hpp"

namespace Flounder
{
	Block::Block(Chunk *parent, const Vector3 &position, const Colour &colour, const bool &filled) :
		m_parent(parent),
		m_position(new Vector3(position)),
		m_colour(new Colour(colour)),
		m_filled(filled)
	{
	}

	Block::~Block()
	{
		delete m_position;
		delete m_colour;
	}
	void Block::SetChanged()
	{

	}
}
