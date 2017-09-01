#pragma once

#include <iostream>
#include <vector>

#include "Character.hpp"

namespace Flounder
{
	/// <summary>
	/// During the loading of a text this represents one word in the text.
	/// </summary>
	class Word
	{
	private:
		std::vector<Character> m_characters;
		double m_width;
	public:
		/// <summary>
		/// Creates a new word.
		/// </summary>
		Word();

		/// <summary>
		/// Deconstructor for the word.
		/// </summary>
		~Word();

		/// <summary>
		/// Adds a character to the end of the current word and increases the screen-space width of the word.
		/// </summary>
		/// <param name="c"> The character to be added. </param>
		/// <returns> {@code true} if the character has successfully been added to the word. </returns>
		bool AddCharacter(Character c);

		std::vector<Character> GetCharacters() const { return m_characters; }

		double GetWidth() const { return m_width; }
	};
}
