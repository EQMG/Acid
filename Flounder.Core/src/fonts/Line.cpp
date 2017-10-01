#include "Line.hpp"

namespace Flounder
{
	Line::Line(const double &spaceWidth, const double &maxLength) :
		m_maxLength(maxLength),
		m_spaceSize(spaceWidth),
		m_words(std::vector<Word>()),
		m_currentLineLength(0.0)
	{
	}

	Line::~Line()
	{
	}

	bool Line::AddWord(Word word)
	{
		double additionalLength = word.GetWidth();
		additionalLength += !m_words.empty() ? m_spaceSize : 0.0;

		if (m_currentLineLength + additionalLength <= m_maxLength)
		{
			m_words.push_back(word);
			m_currentLineLength += additionalLength;
			return true;
		}

		return false;
	}
}
