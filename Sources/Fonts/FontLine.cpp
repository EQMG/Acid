#include "FontLine.hpp"

namespace acid
{
	FontLine::FontLine(const float &spaceWidth, const float &maxLength) :
		m_maxLength(maxLength),
		m_spaceSize(spaceWidth),
		m_words(std::vector<FontWord>()),
		m_currentWordsLength(0.0),
		m_currentLineLength(0.0)
	{
	}

	bool FontLine::AddWord(const FontWord &word)
	{
		float additionalLength = word.GetWidth();
		additionalLength += !m_words.empty() ? m_spaceSize : 0.0f;

		if (m_currentLineLength + additionalLength <= m_maxLength)
		{
			m_words.emplace_back(word);
			m_currentWordsLength += word.GetWidth();
			m_currentLineLength += additionalLength;
			return true;
		}

		return false;
	}
}
