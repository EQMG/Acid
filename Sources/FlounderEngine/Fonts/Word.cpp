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

	bool Word::AddCharacter(Character c, const float &kerning)
	{
		m_characters.push_back(c);
		m_width += kerning + c.GetAdvanceX();
		return true;
	}
}
