#include "Word.hpp"

namespace Flounder
{
	Word::Word() :
		m_characters(std::vector<Character>()),
		m_width(0.0)
	{
	}

	Word::~Word()
	{
	}

	bool Word::AddCharacter(Character c)
	{
		m_characters.push_back(c);
		m_width += c.GetAdvanceX();
		return true;
	}
}
