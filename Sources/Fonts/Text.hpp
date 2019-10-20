#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Visual/Driver.hpp"
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
		std::shared_ptr<FontType> fontType = FontType::Create("Fonts/ProximaNova-Regular.ttf"), Justify justify = Justify::Left,
		const Colour &textColour = Colour::Black, float kerning = 0.0f, float leading = 0.0f);

	void UpdateObject() override;

	/**
	 * Gets the font size.
	 * @return The font size.
	 */
	float GetFontSize() const { return m_fontSize; }

	/**
	 * Sets the font size.
	 * @param fontSize The new font size,
	 */
	void SetFontSize(float fontSize) { m_fontSize = fontSize; }

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
	void SetString(const std::string &string) { m_string = string; }

	/**
	 * Gets how the text should justify.
	 * @return How the text should justify.
	 */
	Justify GetJustify() const { return m_justify; }

	/**
	 * Gets the kerning (type character spacing multiplier) of this text.
	 * @return The type kerning.
	 */
	float GetKerning() const { return m_kerning; }

	/**
	 * Sets the kerning (type character spacing multiplier) of this text.
	 * @param kerning The new kerning.
	 */
	void SetKerning(float kerning) { m_kerning = kerning; }

	/**
	 * Gets the leading (vertical line spacing multiplier) of this text.
	 * @return The line leading.
	 */
	float GetLeading() const { return m_leading; }

	/**
	 * Sets the leading (vertical line spacing multiplier) of this text.
	 * @param leading The new leading.
	 */
	void SetLeading(float leading) { m_leading = leading; }

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

	Driver<float> *GetGlowDriver() const { return m_glowDriver.get(); }

	/**
	 * Sets the glow driver, will disable solid borders.
	 * @param glowDriver The new glow driver.
	 */
	void SetGlowDriver(std::unique_ptr<Driver<float>> &&glowDriver);

	Driver<float> *GetBorderDriver() const { return m_borderDriver.get(); }

	/**
	 * Sets the border driver, will disable glowing.
	 * @param borderDriver The new border driver.
	 */
	void SetBorderDriver(std::unique_ptr<Driver<float>> &&borderDriver);

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

private:
	uint32_t m_numberLines = 0;

	float m_fontSize;
	Justify m_justify;
	std::string m_string;

	std::shared_ptr<FontType> m_fontType;
	float m_kerning;
	float m_leading;

	Colour m_textColour;
	Colour m_borderColour;
	bool m_solidBorder = false;
	bool m_glowBorder = false;

	std::unique_ptr<Driver<float>> m_glowDriver;
	float m_glowSize = 0.0f;

	std::unique_ptr<Driver<float>> m_borderDriver;
	float m_borderSize = 0.0f;
};
}
