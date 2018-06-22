#pragma once

#include <string>
#include <vector>
#include <optional>
#include <map>
#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "FontCharacter.hpp"

namespace fl
{
	/// <summary>
	/// Provides functionality for getting the values from a font file.
	/// </summary>
	class FL_EXPORT FontMetafile :
		public IResource
	{
	private:
		std::map<int, FontCharacter> m_metadata;
		std::map<std::string, std::string> m_values;

		std::string m_filename;
		double m_verticalPerPixelSize;
		double m_horizontalPerPixelSize;
		int m_imageWidth;
		double m_spaceWidth;
		std::vector<int> m_padding;
		int m_paddingWidth;
		int m_paddingHeight;
		double m_maxSizeY;
	public:
		static std::shared_ptr<FontMetafile> Resource(const std::string &filename)
		{
			std::string realFilename = Files::Get()->SearchFile(filename);
			auto resource = Resources::Get()->Get(realFilename);

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<FontMetafile>(resource);
			}

			auto result = std::make_shared<FontMetafile>(realFilename);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static const unsigned int PAD_TOP;
		static const unsigned int PAD_LEFT;
		static const unsigned int PAD_BOTTOM;
		static const unsigned int PAD_RIGHT;
		static const int DESIRED_PADDING;

		static const std::string SPLITTER;
		static const std::string NUMBER_SEPARATOR;

		static const double LINE_HEIGHT;
		static const int SPACE_ASCII;

		/// <summary>
		/// Creates a new meta file.
		/// </summary>
		/// <param name="filepath"> The font file to load from. </param>
		FontMetafile(const std::string &filename);

		/// <summary>
		/// Deconstructor for the meta file.
		/// </summary>
		~FontMetafile();

		std::optional<FontCharacter> GetCharacter(const int &ascii);

		std::string GetFilename() override { return m_filename; }

		double GetSpaceWidth() const { return m_spaceWidth; }

		double GetMaxSizeY() const { return m_maxSizeY; }
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
		int GetValueOfVariable(const std::string &variable);

		/// <summary>
		/// Gets the array of ints associated with a variable on the current line.
		/// </summary>
		/// <param name="variable"> The name of the variable.
		/// </param>
		/// <returns> The int array of values associated with the variable. </returns>
		std::vector<int> GetValuesOfVariable(const std::string &variable);
	};
}
