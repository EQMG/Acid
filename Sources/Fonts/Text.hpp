#pragma once

#include <string>
#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Visual/IDriver.hpp"
#include "Models/Model.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Uis/UiObject.hpp"
#include "Uis/Uis.hpp"
#include "FontType.hpp"
#include "FontLine.hpp"

namespace fl
{
	/// <summary>
	/// A enum that represents how a font will be justified.
	/// </summary>
	enum FontJustify
	{
		JUSTIFY_LEFT = 0,
		JUSTIFY_CENTRE = 1,
		JUSTIFY_RIGHT = 2,
		JUSTIFY_FULLY = 3
	};

	/// <summary>
	/// A object the represents a text in a GUI.
	/// </summary>
	class FL_EXPORT Text :
		public UiObject
	{
	private:
		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformObject;

		Model *m_model;

		std::string m_string;
		std::string m_newString;
		FontJustify m_justify;

		std::shared_ptr<FontType> m_fontType;
		float m_maxWidth;
		float m_kerning;
		float m_leading;

		Colour m_textColour;
		Colour m_borderColour;
		bool m_solidBorder;
		bool m_glowBorder;

		std::shared_ptr<IDriver> m_glowDriver;
		float m_glowSize;

		std::shared_ptr<IDriver> m_borderDriver;
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
		Text(UiObject *parent, const UiBound &rectangle, const float &fontSize, const std::string &text, std::shared_ptr<FontType> fontType = FontType::Resource("Fonts/ProximaNova", FAMILY_REGULAR), const FontJustify &justify = JUSTIFY_LEFT, const float &maxWidth = 1.0f, const float &kerning = 0.0f, const float &leading = 0.0f);

		/// <summary>
		/// Deconstructor for the text.
		/// </summary>
		~Text();

		void UpdateObject() override;

		void CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline);

		/// <summary>
		/// Gets the text model, which contains all the vertex data for the quads on which the text will be rendered.
		/// </summary>
		/// <returns> The model of the text. </returns>
		Model *GetModel() const { return m_model; }

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
		FontJustify GetTextJustify() const { return m_justify; }

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
		/// <param name="leading"> The new kerning. </param>
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

		/// <summary>
		/// Sets the border colour of the text. This is used with border and glow drivers.
		/// </summary>
		/// <param name="borderColour"> The new border colour of the text. </param>
		void SetBorderColour(const Colour &borderColour) { m_borderColour = borderColour; }

		/// <summary>
		/// Sets the border driver, will disable glowing.
		/// </summary>
		/// <param name="driver"> The new border driver. </param>
		void SetBorderDriver(std::shared_ptr<IDriver> driver);

		/// <summary>
		/// Sets a new border driver from a type, will disable glowing.
		/// </summary>
		/// <param name="T"> The type of driver to set. </param>
		/// <param name="args"> The type driver arguments. </param>
		template<typename T, typename... Args>
		void SetBorderDriver(Args &&... args) { SetBorderDriver(std::make_shared<T>(std::forward<Args>(args)...)); }

		/// <summary>
		/// Sets the glow driver, will disable solid borders.
		/// </summary>
		/// <param name="driver"> The new glow driver. </param>
		void SetGlowingDriver(std::shared_ptr<IDriver> driver);

		/// <summary>
		/// Sets a new glow driver from a type, will disable solid borders.
		/// </summary>
		/// <param name="T"> The type of driver to set. </param>
		/// <param name="args"> The type driver arguments. </param>
		template<typename T, typename... Args>
		void SetGlowingDriver(Args &&... args) { SetGlowingDriver(std::make_shared<T>(std::forward<Args>(args)...)); }

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
		/// Gets if the text has been loaded to OpenGL.
		/// </summary>
		/// <returns> If the text has been loaded to OpenGL. </returns>
		bool IsLoaded();

	private:
		/// <summary>
		/// Takes in an unloaded text and calculate all of the vertices for the quads on which this text will be rendered.
		/// The vertex positions and texture coords and calculated based on the information from the font file.
		/// Then takes the information about the vertices of all the quads and stores it in OpenGL.
		/// </summary>
		void LoadText();

		std::vector<FontLine> CreateStructure();

		void CompleteStructure(std::vector<FontLine> &lines, FontLine &currentLine, const FontWord &currentWord);

		std::vector<IVertex *> CreateQuad(const std::vector<FontLine> &lines);

		void AddVerticesForCharacter(const double &cursorX, const double &cursorY, const FontCharacter &character, std::vector<IVertex *> &vertices);

		void AddVertex(const double &vx, const double &vy, const double &tx, const double &ty, std::vector<IVertex *> &vertices);

		void NormalizeQuad(Vector2 *bounding, std::vector<IVertex *> &vertices);
	};
}
