#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Uis/Drivers/UiDriver.hpp"
#include "Models/Model.hpp"
#include "Models/Vertex2d.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Uis/UiObject.hpp"
#include "FontType.hpp"

namespace acid {
/**
 * @brief Class that represents a text in a GUI.
 */
class ACID_EXPORT Text : public UiObject {
	friend class FontType;
public:
	/**
	 * @brief A enum that represents how the text will be justified.
	 */
	enum class Justify {
		Left,
		Centre,
		Right,
		Fully
	};

	/**
	 * Creates a new text object.
	 * @param parent The parent screen object.
	 * @param rectangle The rectangle that will represent the bounds of the ui object.
	 * @param fontSize The font size to be used in this text.
	 * @param text The string text the object will be created with.
	 * @param fontType The font type to be used in this text.
	 * @param justify How the text will justify.
	 * @param textColour The colour of this text.
	 * @param kerning The kerning (type character spacing multiplier) of this text.
	 * @param leading The leading (vertical line spacing multiplier) of this text.
	 */
	Text(UiObject *parent, const UiTransform &rectangle, float fontSize, std::string text,
		std::shared_ptr<FontType> fontType = FontType::Create("Fonts/ProximaNova-Regular.fnt"), Justify justify = Justify::Left,
		const Colour &textColour = Colour::Black, float kerning = 0.0f, float leading = 0.0f);

	void UpdateObject() override;

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline);

	/**
	 * Gets the text model, which contains all the vertex data for the quads on which the text will be rendered.
	 * @return The model of the text.
	 */
	const Model *GetModel() const { return m_model.get(); }

	/**
	 * Gets the font size.
	 * @return The font size.
	 */
	float GetFontSize() const { return m_fontSize; }

	/**
	 * Sets the font size.
	 * @param fontSize The new font size,
	 */
	void SetFontSize(float fontSize);

	/**
	 * Gets the number of lines in this text.
	 * @return The number of lines.
	 */
	uint32_t GetNumberLines() const { return m_numberLines; }

	/**
	 * Gets the string of text represented.
	 * @return The string of text.
	 */
	const std::string &GetString() const { return m_string; }

	/**
	 * Changed the current string in this text.
	 * @param string The new text,
	 */
	void SetString(const std::string &string);

	/**
	 * Gets how the text should justify.
	 * @return How the text should justify.
	 */
	Justify GetJustify() const { return m_justify; }

	/**
	 * Sets how the text should justify.
	 * @param justify How the text will justify.
	 */
	void SetJustify(Justify justify);

	/**
	 * Gets the kerning (type character spacing multiplier) of this text.
	 * @return The type kerning.
	 */
	float GetKerning() const { return m_kerning; }

	/**
	 * Sets the kerning (type character spacing multiplier) of this text.
	 * @param kerning The new kerning.
	 */
	void SetKerning(float kerning);

	/**
	 * Gets the leading (vertical line spacing multiplier) of this text.
	 * @return The line leading.
	 */
	float GetLeading() const { return m_leading; }

	/**
	 * Sets the leading (vertical line spacing multiplier) of this text.
	 * @param leading The new leading.
	 */
	void SetLeading(float leading);

	/**
	 * Gets the font used by this text.
	 * @return The font used by this text.
	 */
	const std::shared_ptr<FontType> &GetFontType() const { return m_fontType; }

	/**
	 * Gets the colour of the text.
	 * @return The colour of the text.
	 */
	const Colour &GetTextColour() const { return m_textColour; }

	/**
	 * Sets the colour of the text.
	 * @param textColour The new colour of the text.
	 */
	void SetTextColour(const Colour &textColour) { m_textColour = textColour; }

	/**
	 * Gets the border colour of the text. This is used with border and glow drivers.
	 * @return The border colour of the text.
	 */
	const Colour &GetBorderColour() const { return m_borderColour; }

	/**
	 * Sets the border colour of the text. This is used with border and glow drivers.
	 * @param borderColour The new border colour of the text.
	 */
	void SetBorderColour(const Colour &borderColour) { m_borderColour = borderColour; }

	UiDriver<float> *GetGlowDriver() const { return m_glowDriver.get(); }
	template<template<typename> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<float> *, UiDriver<float> *>>>
		void SetGlowDriver(Args &&... args) {
		m_glowDriver = std::make_unique<T<float>>(std::forward<Args>(args)...);
		m_solidBorder = false;
		m_glowBorder = true;
	}

	UiDriver<float> *GetBorderDriver() const { return m_borderDriver.get(); }
	template<template<typename> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<float> *, UiDriver<float> *>>>
		void SetBorderDriver(Args &&... args) {
		m_borderDriver = std::make_unique<T<float>>(std::forward<Args>(args)...);
		m_solidBorder = true;
		m_glowBorder = false;
	}

	/**
	 * Disables both solid borders and glow borders.
	 */
	void RemoveBorder();

	/**
	 * Gets the calculated border size.
	 * @return The border size.
	 */
	float GetTotalBorderSize() const;

	/**
	 * Gets the size of the glow.
	 * @return The glow size.
	 */
	float GetGlowSize() const;

	/**
	 * Gets the distance field edge before antialias.
	 * @return The distance field edge.
	 */
	float CalculateEdgeStart() const;

	/**
	 * Gets the distance field antialias distance.
	 * @return The distance field antialias distance.
	 */
	float CalculateAntialiasSize() const;

	/**
	 * Gets if the text has been loaded to a model.
	 * @return If the text has been loaded to a model.
	 */
	bool IsLoaded() const;

private:
	/**
	 * During the loading of a text this represents one word in the text.
	 */
	class Word {
	public:
		/**
		 * Creates a new text word.
		 */
		Word() = default;

		/**
		 * Adds a glyph to the end of the current word and increases the screen-space width of the word.
		 * @param glyph The glyph to be added.
		 * @param kerning The glyph kerning.
		 */
		void AddCharacter(const FontType::Glyph &glyph, float kerning) {
			m_glyphs.emplace_back(glyph);
			m_width += kerning + glyph.m_advance;
		}

		std::vector<FontType::Glyph> m_glyphs;
		float m_width = 0.0f;
	};

	/**
	 * Represents a line of text during the loading of a text.
	 */
	class Line {
	public:
		/**
		 * Creates a new text line.
		 * @param spaceWidth The screen-space width of a space character.
		 * @param maxLength The screen-space maximum length of a line.
		 */
		Line(float spaceWidth, float maxLength) :
			m_maxLength(maxLength),
			m_spaceSize(spaceWidth) {
		}

		/**
		 * Attempt to add a word to the line. If the line can fit the word in without reaching the maximum line length then the word is added and the line length increased.
		 * @param word The word to try to add.
		 * @return {@code true} if the word has successfully been added to the line.
		 */
		bool AddWord(const Word &word) {
			auto additionalLength = word.m_width;
			additionalLength += !m_words.empty() ? m_spaceSize : 0.0f;

			if (m_currentLineLength + additionalLength <= m_maxLength) {
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
		float m_currentWordsLength = 0.0f;
		float m_currentLineLength = 0.0f;
	};

	/**
	 * Takes in an unloaded text and calculate all of the vertices for the quads on which this text will be rendered.
	 * The vertex positions and texture coords and calculated based on the information from the font file.
	 * Then takes the information about the vertices of all the quads and stores it in a model.
	 */
	void LoadText();
	std::vector<Line> CreateStructure() const;
	void CompleteStructure(std::vector<Line> &lines, Line &currentLine, const Word &currentWord, float maxLength) const;
	std::vector<Vertex2d> CreateQuad(const std::vector<Line> &lines) const;
	static void AddVerticesForGlyph(float cursorX, float cursorY, const FontType::Glyph &glyph, std::vector<Vertex2d> &vertices);
	static void AddVertex(float vx, float vy, float tx, float ty, std::vector<Vertex2d> &vertices);

	DescriptorsHandler m_descriptorSet;
	UniformHandler m_uniformObject;

	std::unique_ptr<Model> m_model;
	uint32_t m_numberLines = 0;
	Vector2f m_lastSize;

	float m_fontSize;
	std::string m_string;
	Justify m_justify;
	bool m_dirty = true;

	std::shared_ptr<FontType> m_fontType;
	float m_kerning;
	float m_leading;

	Colour m_textColour;
	Colour m_borderColour;
	bool m_solidBorder = false;
	bool m_glowBorder = false;

	std::unique_ptr<UiDriver<float>> m_glowDriver;
	std::unique_ptr<UiDriver<float>> m_borderDriver;
};
}
