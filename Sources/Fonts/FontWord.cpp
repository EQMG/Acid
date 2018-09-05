#include "FontWord.hpp"

namespace acid
{
	FontWord::FontWord() :
		m_characters(std::vector<FontCharacter>()),
		m_width(0.0)
	{
	}

	bool FontWord::AddCharacter(const FontCharacter &character, const float &kerning)
	{
		m_characters.emplace_back(character);
		m_width += kerning + character.GetAdvanceX();
		return true;
	}
}
