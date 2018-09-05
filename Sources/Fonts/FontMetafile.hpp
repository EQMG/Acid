#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>
#include "Files/Files.hpp"
#include "Resources/IResource.hpp"
#include "FontCharacter.hpp"

namespace acid
{
	/// <summary>
	/// Provides functionality for getting the values from a font file.
	/// </summary>
	class ACID_EXPORT FontMetafile :
		public IResource
	{
	private:
		std::map<int32_t, FontCharacter> m_metadata;
		std::map<std::string, std::string> m_values;

		std::string m_filename;
		float m_verticalPerPixelSize;
		float m_horizontalPerPixelSize;
		int32_t m_imageWidth;
		float m_spaceWidth;
		std::vector<int32_t> m_padding;
		int32_t m_paddingWidth;
		int32_t m_paddingHeight;
		float m_maxSizeY;
	public:
		static const uint32_t PAD_TOP;
		static const uint32_t PAD_LEFT;
		static const uint32_t PAD_BOTTOM;
		static const uint32_t PAD_RIGHT;
		static const int32_t DESIRED_PADDING;

		static const std::string SPLITTER;
		static const std::string NUMBER_SEPARATOR;

		static const float LINE_HEIGHT;
		static const int32_t SPACE_ASCII;

		/// <summary>
		/// Will find an existing metafile with the same filename, or create a new metafile.
		/// </summary>
		/// <param name="filename"> The file to load the metafile from. </param>
		static std::shared_ptr<FontMetafile> Resource(const std::string &filename);

		/// <summary>
		/// Creates a new meta file.
		/// </summary>
		/// <param name="filepath"> The font file to load from. </param>
		FontMetafile(const std::string &filename);

		std::optional<FontCharacter> GetCharacter(const int32_t &ascii);

		std::string GetFilename() override { return m_filename; }

		float GetSpaceWidth() const { return m_spaceWidth; }

		float GetMaxSizeY() const { return m_maxSizeY; }
	private:
		/// <summary>
		/// Read in the next line and store the variable values.
		/// </summary>
		/// <param name="line"> The line to process. </param>
		void ProcessNextLine(const std::string &line);

		/// <summary>
		/// Loads the data about how much padding is used around each character in the texture atlas.
		/// </summary>
		void LoadPaddingData();

		/// <summary>
		/// Loads information about the line height for this font in pixels,
		/// and uses this as a way to find the conversion rate between pixels in the texture atlas and screen-space.
		/// </summary>
		void LoadLineSizes();

		/// <summary>
		/// Loads in data about each character from the texture atlas and converts it all from 'pixels' to 'screen-space' before storing. And stores the data in the <seealso cref="Character"/> class.
		/// The effects of padding are also removed from the data.
		/// </summary>
		void LoadCharacterData();

		/// <summary>
		/// Gets the {@code int} value of the variable with a certain name on the current line.
		/// </summary>
		/// <param name="variable"> The name of the variable.
		/// </param>
		/// <returns> The value of the variable. </returns>
		int32_t GetValueOfVariable(const std::string &variable);

		/// <summary>
		/// Gets the array of ints associated with a variable on the current line.
		/// </summary>
		/// <param name="variable"> The name of the variable.
		/// </param>
		/// <returns> The int32_t array of values associated with the variable. </returns>
		std::vector<int32_t> GetValuesOfVariable(const std::string &variable);
	};
}
