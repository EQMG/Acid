#include "Block.hpp"

#include "Chunk.hpp"

namespace Flounder
{
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
}
