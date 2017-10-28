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

		std::string m_textString;
		Justify m_textJustify;

		std::string m_newText;

		Model *m_model;

		float m_lineMaxSize;
		int m_numberOfLines;

		FontType *m_fonttype;

		Colour *m_textColour;
		Colour *m_borderColour;

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
		/// <param name="maxLineLength"> The longest line length before the text is wrapped, 1.0 being 100% of the screen width when font size = 1. </param>
		/// <param name="align"> How the text will align if wrapped. </param>
		Text(UiObject *parent, const Vector3 &position, const Vector2 &pivot, const std::string &text, const float &fontSize, FontType *fonttype, const float &maxLineLength, const Justify &justify);

		/// <summary>
		/// Deconstructor for the text.
		/// </summary>
		~Text();

		void UpdateObject() override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline);

		/// <summary>
		/// Gets the string of text represented.
		/// </summary>
		/// <returns> The string of text. </returns>
		std::string GetText() const { return m_textString; }

		/// <summary>
		/// Changed the current string in this text.
		/// </summary>
		/// <param name="newText"> The new text, </param>
		void SetText(const std::string &newText);

		/// <summary>
		/// Gets how the text should justify.
		/// </summary>
		/// <returns> How the text should justify. </returns>
		Justify GetTextJustify() const { return m_textJustify; }

		/// <summary>
		/// Gets the text model, which contains all the vertex data for the quads on which the text will be rendered.
		/// </summary>
		/// <returns> The model of the text. </returns>
		Model *GetModel() const { return m_model; }

		/// <summary>
		/// Sets the loaded mesh data for the text.
		/// </summary>
		/// <param name="model"> The mesh model. </param>
		void SetModel(Model *model) { m_model = model; }

		/// <summary>
		/// Gets font type texture for this text.
		/// </summary>
		/// <returns> The texts texture. </returns>
		Texture *GetTexture() const { return m_fonttype->GetTexture(); }

		/// <summary>
		/// Gets the maximum length of a line of this text.
		/// </summary>
		/// <returns> The maximum length of a line. </returns>
		float GetMaxLineSize() const { return m_lineMaxSize; }

		/// <summary>
		/// Gets the number of lines of text. This is determined when the text is  loaded, based on the length of the text and the max line length that is set.
		/// </summary>
		/// <returns> The number of lines of text </returns>
		int GetNumberOfLines() const { return m_numberOfLines; }

		/// <summary>
		/// Sets the number of lines that this text covers (method used only in loading).
		/// </summary>
		/// <param name="number"> The new number of lines. </param>
		void SetNumberOfLines(const int &number) { m_numberOfLines = number; }

		/// <summary>
		/// Gets the font used by this text.
		/// </summary>
		/// <returns> The font used by this text. </returns>
		FontType *GetFontType() const { return m_fonttype; }

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
