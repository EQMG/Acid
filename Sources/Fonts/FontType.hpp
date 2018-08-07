#pragma once

#include "Helpers/FormatString.hpp"
#include "Resources/Resources.hpp"
#include "Textures/Texture.hpp"
#include "FontMetafile.hpp"

namespace acid
{
	/// <summary>
	/// A loader capable of loading font data into a instance of a text mesh.
	/// </summary>
	class ACID_EXPORT FontType :
		public IResource
	{
	private:
		std::string m_name;

		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<FontMetafile> m_metadata;
	public:
		static std::shared_ptr<FontType> Resource(const std::string &filename, const std::string &fontStyle)
		{
			auto resource = Resources::Get()->Get(ToFilename(filename, fontStyle));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<FontType>(resource);
			}

			auto result = std::make_shared<FontType>(filename, fontStyle);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<FontType> Resource(const std::string &data)
		{
			auto split = FormatString::Split(data, "_");
			std::string filename = split[1];
			std::string fontStyle = split[2];
			return Resource(filename, fontStyle);
		}

		/// <summary>
		/// Creates a new text loader.
		/// </summary>
		/// <param name="filename"> The family file path that the texture atlases and character infos are contained in. </param>
		/// <param name="fontStyle"> The style selected to load as this type. </param>
		FontType(const std::string &filename, const std::string &fontStyle);

		/// <summary>
		/// Deconstructor for the font type.
		/// </summary>
		~FontType();

		std::string GetFilename() override { return m_name; }

		std::shared_ptr<Texture> GetTexture() const { return m_texture; }

		std::shared_ptr<FontMetafile> GetMetadata() const { return m_metadata; }
	private:
		static std::string ToFilename(const std::string &filename, const std::string &fontStyle);
	};
}
