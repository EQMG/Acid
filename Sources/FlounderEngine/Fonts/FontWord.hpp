#pragma once

#include <vector>
#include "FontCharacter.hpp"

namespace fl
{
	/// <summary>
	/// During the loading of a text this represents one word in the text.
	/// </summary>
	class FL_EXPORT FontWord
	{
	private:
		std::vector<FontCharacter *> m_characters;
		double m_width;
	public:
		/// <summary>
		/// Creates a new word.
		/// </summary>
		FontWord();

		/// <summary>
		/// Deconstructor for the word.
		/// </summary>
		~FontWord();

		/// <summary>
		/// Adds a character to the end of the current word and increases the screen-space width of the word.
		/// </summary>
		/// <param name="c"> The character to be added. </param>
		/// <param name="kerning"> The character kerning. </param>
		/// <returns> {@code true} if the character has successfully been added to the word. </returns>
		bool AddCharacter(FontCharacter *c, const float &kerning);

		std::vector<FontCharacter *> GetCharacters() const { return m_characters; }

		double GetWidth() const { return m_width; }
	};
}
