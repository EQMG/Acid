#include "FontLine.hpp"

namespace fl
{
	FontLine::FontLine(const double &spaceWidth, const double &maxLength) :
		m_maxLength(maxLength),
		m_spaceSize(spaceWidth),
		m_words(std::vector<FontWord *>()),
		m_currentWordsLength(0.0),
		m_currentLineLength(0.0)
	{
	}

	FontLine::~FontLine()
	{
		for (auto word : m_words)
		{
			delete word;
		}
	}

	bool FontLine::AddWord(FontWord *word)
	{
		double additionalLength = word->GetWidth();
		additionalLength += !m_words.empty() ? m_spaceSize : 0.0;

		if (m_currentLineLength + additionalLength <= m_maxLength)
		{
			m_words.push_back(word);
			m_currentWordsLength += word->GetWidth();
			m_currentLineLength += additionalLength;
			return true;
		}

		return false;
	}
}
