#pragma once

#include <string>
#include "../maths/Colour.hpp"
#include "../maths/Vector2.hpp"
#include "../models/Model.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"
#include "../uis/UiObject.hpp"
#include "../visual/IDriver.hpp"
#include "FontType.hpp"
#include "Justify.hpp"
#include "Line.hpp"

namespace Flounder
{
	/// <summary>
	/// A object the represents a text in a GUI.
	/// </summary>
	class Text :
		public UiObject
	{
	private:
		UniformBuffer *m_uniformObject;
		Model *m_model;

		std::string m_string;
		std::string m_newString;
		Justify m_justify;

		FontType *m_fonttype;
		Colour *m_textColour;
		Colour *m_borderColour;

		float m_kerning;
		float m_leading;
		float m_maxWidth;

		bool m_solidBorder;
		bool m_glowBorder;

		IDriver *m_glowDriver;
		float m_glowSize;

		IDriver *m_borderDriver;
		float m_borderSize;
	public:
		/// <summary>
		/// Creates a new text object.
		/// </summary>
		/// <param name="fonttype"> The text that will be set to this object. </param>
		/// <param name="fontSize"> The initial size of the font (1 is the default). </param>
		/// <param name="font"> The font type to be used in this text. </param>
		/// <param name="align"> How the text will align if wrapped. </param>
		Text(UiObject *parent, const Vector3 &position, const Vector2 &pivot, const std::string &text, const float &fontSize, FontType *fonttype, const Justify &justify);

		/// <summary>
		/// Deconstructor for the text.
		/// </summary>
		~Text();

		void UpdateObject() override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline);

		void RecreateMesh();

		/// <summary>
		/// Gets the text model, which contains all the vertex data for the quads on which the text will be rendered.
		/// </summary>
		/// <returns> The model of the text. </returns>
		Model *GetModel() const { return m_model; }

		/// <summary>
		/// Gets the string of text represented.
		/// </summary>
		/// <returns> The string of text. </returns>
		std::string GetText() const { return m_string; }

		/// <summary>
		/// Changed the current string in this text.
		/// </summary>
		/// <param name="newText"> The new text, </param>
		void SetText(const std::string &newText);

		/// <summary>
		/// Gets how the text should justify.
		/// </summary>
		/// <returns> How the text should justify. </returns>
		Justify GetTextJustify() const { return m_justify; }

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
		/// Gets the font used by this text.
		/// </summary>
		/// <returns> The font used by this text. </returns>
		FontType *GetFontType() const { return m_fonttype; }

		/// <summary>
		/// Gets font type texture for this text.
		/// </summary>
		/// <returns> The texts texture. </returns>
		Texture *GetTexture() const { return m_fonttype->GetTexture(); }

		/// <summary>
		/// Gets the colour of the text.
		/// </summary>
		/// <returns> The colour of the text. </returns>
		Colour *GetTextColour() const { return m_textColour; }

		/// <summary>
		/// Sets the colour of the text.
		/// </summary>
		/// <param name="textColour"> The new colour of the text. </param>
		void SetTextColour(const Colour &textColour) const { m_textColour->Set(textColour); }

		/// <summary>
		/// Gets the border colour of the text. This is used with border and glow drivers.
		/// </summary>
		/// <returns> The border colour of the text. </returns>
		Colour *GetBorderColour() const { return m_borderColour; }

		/// <summary>
		/// Sets the border colour of the text. This is used with border and glow drivers.
		/// </summary>
		/// <param name="borderColour"> The new border colour of the text. </param>
		void SetBorderColour(const Colour &borderColour) const { m_borderColour->Set(borderColour); }

		/// <summary>
		/// Sets a new border driver, will disable glowing.
		/// </summary>
		/// <param name="driver"> The new border driver. </param>
		void SetBorder(IDriver *driver);

		/// <summary>
		/// Sets a new glow driver, will disable solid borders.
		/// </summary>
		/// <param name="driver"> The new glow driver. </param>
		void SetGlowing(IDriver *driver);

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
		/// <param name="text"> The unloaded text. </param>
		static void LoadText(Text *object);

		static std::vector<Line> CreateStructure(Text *object);

		static void CompleteStructure(std::vector<Line> &lines, Line &currentLine, const Word &currentWord, Text *object);

		static std::vector<Vertex> CreateQuad(Text *object, std::vector<Line> lines);

		static void AddVerticesForCharacter(const double &cursorX, const double &cursorY, const Character &character, std::vector<Vertex> &vertices);

		static void AddVertex(const double &vx, const double &vy, const double &tx, const double &ty, std::vector<Vertex> &vertices);

		static Vector2 GetBounding(std::vector<Vertex> &vertices);
	};
}
