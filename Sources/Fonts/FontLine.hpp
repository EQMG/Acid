#pragma once

#include "FontWord.hpp"

namespace acid
{
	/// <summary>
	/// Represents a line of text during the loading of a text.
	/// </summary>
	class ACID_EXPORT FontLine
	{
	private:
		float m_maxLength;
		float m_spaceSize;

		std::vector<FontWord> m_words;
		float m_currentWordsLength;
		float m_currentLineLength;
	public:
		/// <summary>
		/// Creates a new text line.
		/// </summary>
		/// <param name="spaceWidth"> The screen-space width of a space character. </param>
		/// <param name="maxLength"> The screen-space maximum length of a line. </param>
		FontLine(const float &spaceWidth, const float &maxLength);

		/// <summary>
		/// Attempt to add a word to the line. If the line can fit the word in without reaching the maximum line length then the word is added and the line length increased.
		/// </summary>
		/// <param name="word"> The word to try to add. </param>
		/// <returns> {@code true} if the word has successfully been added to the line. </returns>
		bool AddWord(const FontWord &word);

		float GetMaxLength() const { return m_maxLength; }

		float GetSpaceSize() const { return m_spaceSize; }

		std::vector<FontWord> GetWords() const { return m_words; }

		float GetCurrentWordsLength() const { return m_currentWordsLength; }

		float GetCurrentLineLength() const { return m_currentLineLength; }
	};
}
