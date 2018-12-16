﻿#pragma once

#include <string>
#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Visual/IDriver.hpp"
#include "Models/Model.hpp"
#include "Models/VertexModel.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Uis/UiObject.hpp"
#include "FontType.hpp"

namespace acid
{
	/// <summary>
	/// A enum that represents how the text will be justified.
	/// </summary>
	enum TextJustify
	{
		TEXT_JUSTIFY_LEFT = 0,
		TEXT_JUSTIFY_CENTRE = 1,
		TEXT_JUSTIFY_RIGHT = 2,
		TEXT_JUSTIFY_FULLY = 3
	};

	/// <summary>
	/// During the loading of a text this represents one word in the text.
	/// </summary>
	class ACID_EXPORT FontWord
	{
	private:
		std::vector<FontCharacter> m_characters;
		float m_width;
	public:
		/// <summary>
		/// Creates a new text word.
		/// </summary>
		FontWord() :
			m_characters(std::vector<FontCharacter>()),
			m_width(0.0)
		{
		}

		/// <summary>
		/// Adds a character to the end of the current word and increases the screen-space width of the word.
		/// </summary>
		/// <param name="character"> The character to be added. </param>
		/// <param name="kerning"> The character kerning. </param>
		void AddCharacter(const FontCharacter &character, const float &kerning)
		{
			m_characters.emplace_back(character);
			m_width += kerning + character.GetAdvanceX();
		}

		std::vector<FontCharacter> GetCharacters() const { return m_characters; }

		float GetWidth() const { return m_width; }
	};

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
		FontLine(const float &spaceWidth, const float &maxLength) :
			m_maxLength(maxLength),
			m_spaceSize(spaceWidth),
			m_words(std::vector<FontWord>()),
			m_currentWordsLength(0.0),
			m_currentLineLength(0.0)
		{
		}

		/// <summary>
		/// Attempt to add a word to the line. If the line can fit the word in without reaching the maximum line length then the word is added and the line length increased.
		/// </summary>
		/// <param name="word"> The word to try to add. </param>
		/// <returns> {@code true} if the word has successfully been added to the line. </returns>
		bool AddWord(const FontWord &word)
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

		float GetMaxLength() const { return m_maxLength; }

		float GetSpaceSize() const { return m_spaceSize; }

		std::vector<FontWord> GetWords() const { return m_words; }

		float GetCurrentWordsLength() const { return m_currentWordsLength; }

		float GetCurrentLineLength() const { return m_currentLineLength; }
	};

	/// <summary>
	/// A object the represents a text in a GUI.
	/// </summary>
	class ACID_EXPORT Text :
		public UiObject
	{
	private:
		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformObject;

		std::unique_ptr<Model> m_model;
		uint32_t m_numberLines;

		std::string m_string;
		std::string m_newString;
		TextJustify m_justify;

		std::shared_ptr<FontType> m_fontType;
		float m_maxWidth;
		float m_kerning;
		float m_leading;

		Colour m_textColour;
		Colour m_borderColour;
		bool m_solidBorder;
		bool m_glowBorder;

		std::unique_ptr<IDriver> m_glowDriver;
		float m_glowSize;

		std::unique_ptr<IDriver> m_borderDriver;
		float m_borderSize;
	public:
		/// <summary>
		/// Creates a new text object.
		/// </summary>
		/// <param name="parent"> The parent screen object. </param>
		/// <param name="rectangle"> The rectangle that will represent the bounds of the ui object. </param>
		/// <param name="fontSize"> The font size to be used in this text. </param>
		/// <param name="text"> The string text the object will be created with. </param>
		/// <param name="fontType"> The font type to be used in this text. </param>
		/// <param name="justify"> How the text will justify. </param>
		/// <param name="maxWidth"> The maximum length of a line of this text. </param>
		/// <param name="kerning"> The kerning (type character spacing multiplier) of this text. </param>
		/// <param name="leading"> The leading (vertical line spacing multiplier) of this text. </param>
		Text(UiObject *parent, const UiBound &rectangle, const float &fontSize, const std::string &text, const std::shared_ptr<FontType> &fontType = FontType::Resource("Fonts/ProximaNova", "Regular"), const TextJustify &justify = TEXT_JUSTIFY_LEFT, const float &maxWidth = 1.0f, const float &kerning = 0.0f, const float &leading = 0.0f);

		void UpdateObject() override;

		bool CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler &uniformScene);

		/// <summary>
		/// Gets the text model, which contains all the vertex data for the quads on which the text will be rendered.
		/// </summary>
		/// <returns> The model of the text. </returns>
		Model *GetModel() const { return m_model.get(); }

		/// <summary>
		/// Gets the number of lines in this text.
		/// </summary>
		/// <returns> The number of lines. </returns>
		uint32_t GetNumberLines() const { return m_numberLines; }

		/// <summary>
		/// Gets the string of text represented.
		/// </summary>
		/// <returns> The string of text. </returns>
		std::string GetString() const { return m_string; }

		/// <summary>
		/// Changed the current string in this text.
		/// </summary>
		/// <param name="newString"> The new text, </param>
		void SetString(const std::string &newString);

		/// <summary>
		/// Gets how the text should justify.
		/// </summary>
		/// <returns> How the text should justify. </returns>
		TextJustify GetTextJustify() const { return m_justify; }

		/// <summary>
		/// Gets the maximum length of a line of this text.
		/// </summary>
		/// <returns> The maximum length of a line. </returns>
		float GetMaxWidth() const { return m_maxWidth; }

		/// <summary>
		/// Sets the maximum length of a line of this text.
		/// </summary>
		/// <param name="maxWidth"> The new maximum length. </param>
		void SetMaxWidth(const float &maxWidth) { m_maxWidth = maxWidth; }

		/// <summary>
		/// Gets the kerning (type character spacing multiplier) of this text.
		/// </summary>
		/// <returns> The type kerning. </returns>
		float GetKerning() const { return m_kerning; }

		/// <summary>
		/// Sets the kerning (type character spacing multiplier) of this text.
		/// </summary>
		/// <param name="kerning"> The new kerning. </param>
		void SetKerning(const float &kerning) { m_kerning = kerning; }

		/// <summary>
		/// Gets the leading (vertical line spacing multiplier) of this text.
		/// </summary>
		/// <returns> The line leading. </returns>
		float GetLeading() const { return m_leading; }

		/// <summary>
		/// Sets the leading (vertical line spacing multiplier) of this text.
		/// </summary>
		/// <param name="leading"> The new leading. </param>
		void SetLeading(const float &leading) { m_leading = leading; }

		/// <summary>
		/// Gets the font used by this text.
		/// </summary>
		/// <returns> The font used by this text. </returns>
		std::shared_ptr<FontType> GetFontType() const { return m_fontType; }

		/// <summary>
		/// Gets font type texture for this text.
		/// </summary>
		/// <returns> The texts texture. </returns>
		std::shared_ptr<Texture> GetTexture() const { return m_fontType->GetTexture(); }

		/// <summary>
		/// Gets the colour of the text.
		/// </summary>
		/// <returns> The colour of the text. </returns>
		Colour GetTextColour() const { return m_textColour; }

		/// <summary>
		/// Sets the colour of the text.
		/// </summary>
		/// <param name="textColour"> The new colour of the text. </param>
		void SetTextColour(const Colour &textColour) { m_textColour = textColour; }

		/// <summary>
		/// Gets the border colour of the text. This is used with border and glow drivers.
		/// </summary>
		/// <returns> The border colour of the text. </returns>
		Colour GetBorderColour() const { return m_borderColour; }

		IDriver *GetBorderDriver() const { return m_borderDriver.get(); }

		/// <summary>
		/// Sets the border colour of the text. This is used with border and glow drivers.
		/// </summary>
		/// <param name="borderColour"> The new border colour of the text. </param>
		void SetBorderColour(const Colour &borderColour) { m_borderColour = borderColour; }

		/// <summary>
		/// Sets the border driver, will disable glowing.
		/// </summary>
		/// <param name="borderDriver"> The new border driver. </param>
		void SetBorderDriver(IDriver *borderDriver);

		/// <summary>
		/// Sets a new border driver from a type, will disable glowing.
		/// </summary>
		/// <param name="T"> The type of driver to set. </param>
		/// <param name="args"> The type driver arguments. </param>
		template<typename T, typename... Args>
		void SetBorderDriver(Args &&... args) { SetBorderDriver(new T(std::forward<Args>(args)...)); }

		IDriver *GetGlowDriver() const { return m_glowDriver.get(); }

		/// <summary>
		/// Sets the glow driver, will disable solid borders.
		/// </summary>
		/// <param name="glowDriver"> The new glow driver. </param>
		void SetGlowDriver(IDriver *glowDriver);

		/// <summary>
		/// Sets a new glow driver from a type, will disable solid borders.
		/// </summary>
		/// <param name="T"> The type of driver to set. </param>
		/// <param name="args"> The type driver arguments. </param>
		template<typename T, typename... Args>
		void SetGlowDriver(Args &&... args) { SetGlowDriver(new T(std::forward<Args>(args)...)); }

		/// <summary>
		/// Disables both solid borders and glow borders.
		/// </summary>
		void RemoveBorder();

		/// <summary>
		/// Gets the calculated border size.
		/// </summary>
		/// <returns> The border size. </returns>
		float GetTotalBorderSize();

		/// <summary>
		/// Gets the size of the glow.
		/// </summary>
		/// <returns> The glow size. </returns>
		float GetGlowSize();

		/// <summary>
		/// Gets the distance field edge before antialias.
		/// </summary>
		/// <returns> The distance field edge. </returns>
		float CalculateEdgeStart();

		/// <summary>
		/// Gets the distance field antialias distance.
		/// </summary>
		/// <returns> The distance field antialias distance. </returns>
		float CalculateAntialiasSize();

		/// <summary>
		/// Gets if the text has been loaded to a model.
		/// </summary>
		/// <returns> If the text has been loaded to a model. </returns>
		bool IsLoaded();
	private:
		/// <summary>
		/// Takes in an unloaded text and calculate all of the vertices for the quads on which this text will be rendered.
		/// The vertex positions and texture coords and calculated based on the information from the font file.
		/// Then takes the information about the vertices of all the quads and stores it in a model.
		/// </summary>
		void LoadText();

		std::vector<FontLine> CreateStructure();

		void CompleteStructure(std::vector<FontLine> &lines, FontLine &currentLine, const FontWord &currentWord);

		std::vector<VertexModel> CreateQuad(const std::vector<FontLine> &lines);

		void AddVerticesForCharacter(const float &cursorX, const float &cursorY, const FontCharacter &character, std::vector<VertexModel> &vertices);

		void AddVertex(const float &vx, const float &vy, const float &tx, const float &ty, std::vector<VertexModel> &vertices);

		void NormalizeQuad(Vector2 &bounding, std::vector<VertexModel> &vertices);
	};
}
