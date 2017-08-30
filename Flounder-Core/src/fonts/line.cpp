#include "line.hpp"

namespace flounder
{
	line::line(const double &spaceWidth, const double &maxLength) :
		m_maxLength(maxLength),
		m_spaceSize(spaceWidth),
		m_words(new std::vector<word*>()),
		m_currentLineLength(0.0)
	{
	}

	line::~line()
	{
		delete m_words;
	}

	bool line::addWord(word *word)
	{
		double additionalLength = word->getWidth();
		additionalLength += !m_words->empty() ? m_spaceSize : 0.0;

		if (m_currentLineLength + additionalLength <= m_maxLength)
		{
			m_words->push_back(word);
			m_currentLineLength += additionalLength;
			return true;
		}

		return false;
	}
}
