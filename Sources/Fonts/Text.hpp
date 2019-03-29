#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Visual/IDriver.hpp"
#include "Models/Model.hpp"
#include "Models/VertexModel.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Uis/UiObject.hpp"
#include "FontType.hpp"

namespace acid
{
	/// <summary>
	/// A object the represents a text in a GUI.
	/// </summary>
	class ACID_EXPORT Text :
		public UiObject
	{
	public:
		/// <summary>
		/// A enum that represents how the text will be justified.
		/// </summary>
		enum class Justify
		{
			Left, Centre, Right, Fully
		};

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
		/// <param name="textColour"> The colour of this text. </param>
		/// <param name="kerning"> The kerning (type character spacing multiplier) of this text. </param>
		/// <param name="leading"> The leading (vertical line spacing multiplier) of this text. </param>
		Text(UiObject *parent, const UiBound &rectangle, const float &fontSize, std::string text, std::shared_ptr<FontType> fontType = FontType::Create("Fonts/ProximaNova", "Regular"),
			const Justify &justify = Justify::Left, const float &maxWidth = 1.0f, const Colour &textColour = Colour::Black, const float &kerning = 0.0f, const float &leading = 0.0f);

		void UpdateObject() override;

		bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene);

		/// <summary>
		/// Gets the text model, which contains all the vertex data for the quads on which the text will be rendered.
		/// </summary>
		/// <returns> The model of the text. </returns>
		const Model *GetModel() const { return m_model.get(); }

		/// <summary>
		/// Gets the number of lines in this text.
		/// </summary>
		/// <returns> The number of lines. </returns>
		const uint32_t &GetNumberLines() const { return m_numberLines; }

		/// <summary>
		/// Gets the string of text represented.
		/// </summary>
		/// <returns> The string of text. </returns>
		const std::string &GetString() const { return m_string; }

		/// <summary>
		/// Changed the current string in this text.
		/// </summary>
		/// <param name="newString"> The new text, </param>
		void SetString(const std::string &newString);

		/// <summary>
		/// Gets how the text should justify.
		/// </summary>
		/// <returns> How the text should justify. </returns>
		const Justify &GetJustify() const { return m_justify; }

		/// <summary>
		/// Gets the maximum length of a line of this text.
		/// </summary>
		/// <returns> The maximum length of a line. </returns>
		const float &GetMaxWidth() const { return m_maxWidth; }

		/// <summary>
		/// Sets the maximum length of a line of this text.
		/// </summary>
		/// <param name="maxWidth"> The new maximum length. </param>
		void SetMaxWidth(const float &maxWidth) { m_maxWidth = maxWidth; }

		/// <summary>
		/// Gets the kerning (type character spacing multiplier) of this text.
		/// </summary>
		/// <returns> The type kerning. </returns>
		const float &GetKerning() const { return m_kerning; }

		/// <summary>
		/// Sets the kerning (type character spacing multiplier) of this text.
		/// </summary>
		/// <param name="kerning"> The new kerning. </param>
		void SetKerning(const float &kerning) { m_kerning = kerning; }

		/// <summary>
		/// Gets the leading (vertical line spacing multiplier) of this text.
		/// </summary>
		/// <returns> The line leading. </returns>
		const float &GetLeading() const { return m_leading; }

		/// <summary>
		/// Sets the leading (vertical line spacing multiplier) of this text.
		/// </summary>
		/// <param name="leading"> The new leading. </param>
		void SetLeading(const float &leading) { m_leading = leading; }

		/// <summary>
		/// Gets the font used by this text.
		/// </summary>
		/// <returns> The font used by this text. </returns>
		const std::shared_ptr<FontType> &GetFontType() const { return m_fontType; }

		/// <summary>
		/// Gets the colour of the text.
		/// </summary>
		/// <returns> The colour of the text. </returns>
		const Colour &GetTextColour() const { return m_textColour; }

		/// <summary>
		/// Sets the colour of the text.
		/// </summary>
		/// <param name="textColour"> The new colour of the text. </param>
		void SetTextColour(const Colour &textColour) { m_textColour = textColour; }

		/// <summary>
		/// Gets the border colour of the text. This is used with border and glow drivers.
		/// </summary>
		/// <returns> The border colour of the text. </returns>
		const Colour &GetBorderColour() const { return m_borderColour; }

		/// <summary>
		/// Sets the border colour of the text. This is used with border and glow drivers.
		/// </summary>
		/// <param name="borderColour"> The new border colour of the text. </param>
		void SetBorderColour(const Colour &borderColour) { m_borderColour = borderColour; }

		IDriver<float> *GetGlowDriver() const { return m_glowDriver.get(); }

		/// <summary>
		/// Sets the glow driver, will disable solid borders.
		/// </summary>
		/// <param name="glowDriver"> The new glow driver. </param>
		void SetGlowDriver(IDriver<float> *glowDriver);

		IDriver<float> *GetBorderDriver() const { return m_borderDriver.get(); }

		/// <summary>
		/// Sets the border driver, will disable glowing.
		/// </summary>
		/// <param name="borderDriver"> The new border driver. </param>
		void SetBorderDriver(IDriver<float> *borderDriver);

		/// <summary>
		/// Disables both solid borders and glow borders.
		/// </summary>
		void RemoveBorder();

		/// <summary>
		/// Gets the calculated border size.
		/// </summary>
		/// <returns> The border size. </returns>
		float GetTotalBorderSize() const;

		/// <summary>
		/// Gets the size of the glow.
		/// </summary>
		/// <returns> The glow size. </returns>
		float GetGlowSize() const;

		/// <summary>
		/// Gets the distance field edge before antialias.
		/// </summary>
		/// <returns> The distance field edge. </returns>
		float CalculateEdgeStart() const;

		/// <summary>
		/// Gets the distance field antialias distance.
		/// </summary>
		/// <returns> The distance field antialias distance. </returns>
		float CalculateAntialiasSize() const;

		/// <summary>
		/// Gets if the text has been loaded to a model.
		/// </summary>
		/// <returns> If the text has been loaded to a model. </returns>
		bool IsLoaded() const;
	private:
		/// <summary>
		/// During the loading of a text this represents one word in the text.
		/// </summary>
		class Word
		{
		public:
			/// <summary>
			/// Creates a new text word.
			/// </summary>
			Word() :
				m_width(0.0f)
			{
			}

			/// <summary>
			/// Adds a character to the end of the current word and increases the screen-space width of the word.
			/// </summary>
			/// <param name="character"> The character to be added. </param>
			/// <param name="kerning"> The character kerning. </param>
			void AddCharacter(const FontMetafile::Character &character, const float &kerning)
			{
				m_characters.emplace_back(character);
				m_width += kerning + character.m_advanceX;
			}

			std::vector<FontMetafile::Character> m_characters;
			float m_width;
		};

		/// <summary>
		/// Represents a line of text during the loading of a text.
		/// </summary>
		class Line
		{
		public:
			/// <summary>
			/// Creates a new text line.
			/// </summary>
			/// <param name="spaceWidth"> The screen-space width of a space character. </param>
			/// <param name="maxLength"> The screen-space maximum length of a line. </param>
			Line(const float &spaceWidth, const float &maxLength) :
				m_maxLength(maxLength),
				m_spaceSize(spaceWidth),
				m_currentWordsLength(0.0f),
				m_currentLineLength(0.0f)
			{
			}

			/// <summary>
			/// Attempt to add a word to the line. If the line can fit the word in without reaching the maximum line length then the word is added and the line length increased.
			/// </summary>
			/// <param name="word"> The word to try to add. </param>
			/// <returns> {@code true} if the word has successfully been added to the line. </returns>
			bool AddWord(const Word &word)
			{
				float additionalLength = word.m_width;
				additionalLength += !m_words.empty() ? m_spaceSize : 0.0f;

				if (m_currentLineLength + additionalLength <= m_maxLength)
				{
					m_words.emplace_back(word);
					m_currentWordsLength += word.m_width;
					m_currentLineLength += additionalLength;
					return true;
				}

				return false;
			}

			float m_maxLength;
			float m_spaceSize;

			std::vector<Word> m_words;
			float m_currentWordsLength;
			float m_currentLineLength;
		};

		/// <summary>
		/// Takes in an unloaded text and calculate all of the vertices for the quads on which this text will be rendered.
		/// The vertex positions and texture coords and calculated based on the information from the font file.
		/// Then takes the information about the vertices of all the quads and stores it in a model.
		/// </summary>
		void LoadText();

		std::vector<Line> CreateStructure() const;

		void CompleteStructure(std::vector<Line> &lines, Line &currentLine, const Word &currentWord) const;

		std::vector<VertexModel> CreateQuad(const std::vector<Line> &lines);

		static void AddVerticesForCharacter(const float &cursorX, const float &cursorY, const FontMetafile::Character &character, std::vector<VertexModel> &vertices);

		static void AddVertex(const float &vx, const float &vy, const float &tx, const float &ty, std::vector<VertexModel> &vertices);

		void NormalizeQuad(Vector2 &bounding, std::vector<VertexModel> &vertices) const;

		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformObject;

		std::unique_ptr<Model> m_model;
		uint32_t m_numberLines;

		std::string m_string;
		std::optional<std::string> m_newString;
		Justify m_justify;

		std::shared_ptr<FontType> m_fontType;
		float m_maxWidth;
		float m_kerning;
		float m_leading;

		Colour m_textColour;
		Colour m_borderColour;
		bool m_solidBorder;
		bool m_glowBorder;

		std::unique_ptr<IDriver<float>> m_glowDriver;
		float m_glowSize;

		std::unique_ptr<IDriver<float>> m_borderDriver;
		float m_borderSize;
	};
}
