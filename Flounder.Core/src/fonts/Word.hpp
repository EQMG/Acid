#pragma once

#include <iostream>
#include <vector>

#include "character.hpp"

namespace Flounder
{
	/// <summary>
	/// During the loading of a text this represents one word in the text.
	/// </summary>
	class word
	{
	private:
		std::vector<character*> *m_characters;
		double m_width;
	public:
		/// <summary>
		/// Creates a new word.
		/// </summary>
		word();

		/// <summary>
		/// Deconstructor for the word.
		/// </summary>
		~word();

		/// <summary>
		/// Adds a character to the end of the current word and increases the screen-space width of the word.
		/// </summary>
		/// <param name="c"> The character to be added. </param>
		/// <returns> {@code true} if the character has successfully been added to the word. </returns>
		bool addCharacter(character *cc);

		std::vector<character*> *getCharacters() const { return m_characters; }

		double getWidth() const { return m_width; }
	};
}
