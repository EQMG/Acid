#include "FontWord.hpp"

namespace fl
{
	FontWord::FontWord() :
		m_characters(std::vector<FontCharacter>()),
		m_width(0.0)
	{
	}

	FontWord::~FontWord()
	{
	}

	bool FontWord::AddCharacter(const FontCharacter &character, const float &kerning)
	{
		m_characters.emplace_back(character);
		m_width += kerning + character.GetAdvanceX();
		return true;
	}
}
