#pragma once

#include "word.hpp"

namespace flounder
{
	/// <summary>
	/// Represents a line of text during the loading of a text.
	/// </summary>
	class line
	{
	private:
		double m_maxLength;
		double m_spaceSize;

		std::vector<word*> *m_words;
		double m_currentLineLength;
	public:
		/// <summary>
		/// Creates a new line.
		/// </summary>
		/// <param name="spaceWidth"> The screen-space width of a space character. </param>
		/// <param name="maxLength"> The screen-space maximum length of a line. </param>
		line(const double &spaceWidth, const double &maxLength);

		/// <summary>
		/// Deconstructor for the line.
		/// </summary>
		~line();

		/// <summary>
		/// Attempt to add a word to the line. If the line can fit the word in without reaching the maximum line length then the word is added and the line length increased.
		/// </summary>
		/// <param name="word"> The word to try to add.
		/// </param>
		/// <returns> {@code true} if the word has successfully been added to the line. </returns>
		bool addWord(word *word);

		double getMaxLength() const { return m_maxLength; }

		double getSpaceSize() const { return m_spaceSize; }

		std::vector<word*> *getWords() const { return m_words; }

		double getCurrentLineLength() const { return m_currentLineLength; }
	};
}
