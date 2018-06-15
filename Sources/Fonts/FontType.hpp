#pragma once

#include "Helpers/FormatString.hpp"
#include "Resources/Resources.hpp"
#include "Textures/Texture.hpp"
#include "FontMetafile.hpp"

namespace fl
{
	enum FamilyType
	{
		FAMILY_THIN = 0,
		FAMILY_LIGHT = 1,
		FAMILY_REGULAR = 2,
		FAMILY_SEMIBOLD = 3,
		FAMILY_BOLD = 4
	};

	/// <summary>
	/// A loader capable of loading font data into a instance of a text mesh.
	/// </summary>
	class FL_EXPORT FontType :
		public IResource
	{
	private:
		std::string m_filename;

		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<FontMetafile> m_metadata;
	public:
		static std::shared_ptr<FontType> Resource(const std::string &filename, const FamilyType &familyType)
		{
			auto resource = Resources::Get()->Get(ToFilename(filename, familyType));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<FontType>(resource);
			}

			auto result = std::make_shared<FontType>(filename, familyType);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<FontType> Resource(const std::string &data)
		{
			auto split = FormatString::Split(data, "_");
			std::string filename = split[1].c_str();
			FamilyType familyType = static_cast<FamilyType>(atoi(split[2].c_str()));
			return Resource(filename, familyType);
		}

		/// <summary>
		/// Creates a new text loader.
		/// </summary>
		/// <param name="textureFile"> The file for the font atlas texture. </param>
		/// <param name="fontFile"> The font file containing information about each character in the texture atlas. </param>
		FontType(const std::string &filename, const FamilyType &familyType);

		/// <summary>
		/// Deconstructor for the font type.
		/// </summary>
		~FontType();

		std::string GetFilename() override { return m_filename; }

		std::shared_ptr<Texture> GetTexture() const { return m_texture; }

		std::shared_ptr<FontMetafile> GetMetadata() const { return m_metadata; }
	private:
		static std::string ToFilename(const std::string &filename, const FamilyType &familyType);

		static std::string FamilyString(const FamilyType &familyType);
	};
}
