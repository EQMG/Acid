#pragma once

#include "Helpers/String.hpp"
#include "Resources/IResource.hpp"
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
		/// <summary>
		/// Will find an existing font type with the same filename, or create a new font type.
		/// </summary>
		/// <param name="filename"> The family file path that the texture atlases and character infos are contained in. </param>
		/// <param name="fontStyle"> The style selected to load as this type. </param>
		static std::shared_ptr<FontType> Resource(const std::string &filename, const std::string &fontStyle);

		/// <summary>
		/// Will find an existing font type with the same filename, or create a new font type.
		/// </summary>
		/// <param name="data"> The combined data for the family file path and selected style. </param>
		static std::shared_ptr<FontType> Resource(const std::string &data);

		/// <summary>
		/// Creates a new text loader.
		/// </summary>
		/// <param name="filename"> The family file path that the texture atlases and character infos are contained in. </param>
		/// <param name="fontStyle"> The style selected to load as this type. </param>
		FontType(const std::string &filename, const std::string &fontStyle);

		std::string GetFilename() override { return m_name; }

		std::shared_ptr<Texture> GetTexture() const { return m_texture; }

		std::shared_ptr<FontMetafile> GetMetadata() const { return m_metadata; }
	private:
		static std::string ToFilename(const std::string &filename, const std::string &fontStyle);
	};
}
