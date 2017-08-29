#pragma once

#include <string>

#include "../framework/framework.hpp"
#include "../maths/colour.hpp"
#include "../maths/vector2.hpp"
#include "../visual/idriver.hpp"
#include "../uis/uialign.hpp"
#include "../uis/uiobject.hpp"
#include "../visual/driverconstant.hpp"

#include "fonttype.hpp"
#include "line.hpp"
#include "../models/model.hpp"

namespace flounder
{
	/// <summary>
	/// A object the represents a text in a GUI.
	/// </summary>
	class text :
		public uiobject
	{
	private:
		std::string m_textString;
		uialign m_textAlign;

		std::string m_newText;

		model *m_model;

		float m_lineMaxSize;
		int m_numberOfLines;

		fonttype *m_fonttype;

		colour *m_textColour;
		colour *m_borderColour;

		bool m_solidBorder;
		bool m_glowBorder;

		idriver *m_glowDriver;
		float m_glowSize;

		idriver *m_borderDriver;
		float m_borderSize;
	public:
		/// <summary>
		/// Creates a new text object.
		/// </summary>
		/// <param name="parent"> The objects parent. </param>
		/// <param name="position"> The objects position relative to the parents. </param>
		/// <param name="fonttype"> The text that will be set to this object. </param>
		/// <param name="fontSize"> The initial size of the font (1 is the default). </param>
		/// <param name="font"> The font type to be used in this text. </param>
		/// <param name="maxLineLength"> The longest line length before the text is wrapped, 1.0 being 100% of the screen width when font size = 1. </param>
		/// <param name="align"> How the text will align if wrapped. </param>
		text(uiobject *parent, const vector2 &position, const std::string &text, const float &fontSize, fonttype *fonttype, const float &maxLineLength, const uialign &align);

		/// <summary>
		/// Deconstructor for the text.
		/// </summary>
		~text();

		void updateObject() override;
	private:
		/// <summary>
		/// Takes in an unloaded text and calculate all of the vertices for the quads on which this text will be rendered.
		/// The vertex positions and texture coords and calculated based on the information from the font file.
		/// Then takes the information about the vertices of all the quads and stores it in OpenGL.
		/// </summary>
		/// <param name="text"> The unloaded text. </param>
		static void loadText(text *object);

		static std::vector<line*> createStructure(text *object);

		static void completeStructure(std::vector<line*> &lines, line *currentLine, word *currentWord, text *object);

		static void createQuadVertices(text *object, std::vector<line*> lines, std::vector<float> *vertices, std::vector<float> *textures);

		static void addVerticesForCharacter(const double &cursorX, const double &cursorY, character *character, std::vector<float> *vertices);

		static void addVertices(const double &x, const double &y, const double &maxX, const double &maxY, std::vector<float> *vertices);

		static void addTextures(const double &x, const double &y, const double &maxX, const double &maxY, std::vector<float> *textures);

		static vector2 getBounding(std::vector<float> *vertices);
	public:
		/// <summary>
		/// Gets the string of text represented.
		/// </summary>
		/// <returns> The string of text. </returns>
		std::string getText() { return m_textString; }

		/// <summary>
		/// Changed the current string in this text.
		/// </summary>
		/// <param name="newText"> The new text, </param>
		void setText(const std::string &newText);

		/// <summary>
		/// Gets how the text should align.
		/// </summary>
		/// <returns> How the text should align. </returns>
		uialign getTextAlign() { return m_textAlign; }

		/// <summary>
		/// Gets the text model, which contains all the vertex data for the quads on which the text will be rendered.
		/// </summary>
		/// <returns> The model of the text. </returns>
		model *getModel() { return m_model; }

		/// <summary>
		/// Sets the loaded mesh data for the text.
		/// </summary>
		/// <param name="model"> The mesh model. </param>
		void setModel(model *model) { m_model = model; }

		/// <summary>
		/// Gets font type texture for this text.
		/// </summary>
		/// <returns> The texts texture. </returns>
		texture *getTexture() { return m_fonttype->getTexture(); }

		/// <summary>
		/// Gets the maximum length of a line of this text.
		/// </summary>
		/// <returns> The maximum length of a line. </returns>
		float getMaxLineSize() { return m_lineMaxSize; }

		/// <summary>
		/// Gets the number of lines of text. This is determined when the text is  loaded, based on the length of the text and the max line length that is set.
		/// </summary>
		/// <returns> The number of lines of text </returns>
		int getNumberOfLines() { return m_numberOfLines; }

		/// <summary>
		/// Sets the number of lines that this text covers (method used only in loading).
		/// </summary>
		/// <param name="number"> The new number of lines. </param>
		void setNumberOfLines(const int &number) { m_numberOfLines = number; }

		/// <summary>
		/// Gets the font used by this text.
		/// </summary>
		/// <returns> The font used by this text. </returns>
		fonttype *getFontType() { return m_fonttype; }

		/// <summary>
		/// Gets the colour of the text.
		/// </summary>
		/// <returns> The colour of the text. </returns>
		colour *getTextColour() { return m_textColour; }

		/// <summary>
		/// Sets the colour of the text.
		/// </summary>
		/// <param name="textColour"> The new colour of the text. </param>
		void setTextColour(const colour &textColour) { m_textColour->set(textColour); }

		/// <summary>
		/// Gets the border colour of the text. This is used with border and glow drivers.
		/// </summary>
		/// <returns> The border colour of the text. </returns>
		colour *getBorderColour() { return m_borderColour; }

		/// <summary>
		/// Sets the border colour of the text. This is used with border and glow drivers.
		/// </summary>
		/// <param name="borderColour"> The new border colour of the text. </param>
		void setBorderColour(const colour &borderColour) { m_borderColour->set(borderColour); }

		/// <summary>
		/// Sets a new border driver, will disable glowing.
		/// </summary>
		/// <param name="driver"> The new border driver. </param>
		void setBorder(idriver *driver);

		/// <summary>
		/// Sets a new glow driver, will disable solid borders.
		/// </summary>
		/// <param name="driver"> The new glow driver. </param>
		void setGlowing(idriver *driver);

		/// <summary>
		/// Disables both solid borders and glow borders.
		/// </summary>
		void removeBorder();

		/// <summary>
		/// Gets the calculated border size.
		/// </summary>
		/// <returns> The border size. </returns>
		float getTotalBorderSize();

		/// <summary>
		/// Gets the size of the glow.
		/// </summary>
		/// <returns> The glow size. </returns>
		float getGlowSize();

		/// <summary>
		/// Gets the distance field edge before antialias.
		/// </summary>
		/// <returns> The distance field edge. </returns>
		float calculateEdgeStart();

		/// <summary>
		/// Gets the distance field antialias distance.
		/// </summary>
		/// <returns> The distance field antialias distance. </returns>
		float calculateAntialiasSize();

		/// <summary>
		/// Gets if the text has been loaded to OpenGL.
		/// </summary>
		/// <returns> If the text has been loaded to OpenGL. </returns>
		bool isLoaded();
	};
}
