#include "FontWord.hpp"

namespace Flounder
{
	FontWord::FontWord() :
		m_characters(std::vector<FontCharacter>()),
		m_width(0.0)
	{
	}

	FontWord::~FontWord()
	{
	}

	bool FontWord::AddCharacter(FontCharacter c, const float &kerning)
	{
		m_characters.push_back(c);
		m_width += kerning + c.GetAdvanceX();
		return true;
	}
}
