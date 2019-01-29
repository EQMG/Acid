#pragma once

#include "Helpers/String.hpp"
#include "Resources/Resource.hpp"
#include "Textures/Texture.hpp"
#include "FontMetafile.hpp"

namespace acid
{
	/// <summary>
	/// A loader capable of loading font data into a instance of a text mesh.
	/// </summary>
	class ACID_EXPORT FontType :
		public Resource
	{
	public:
		static std::shared_ptr<FontType> Create(const Metadata &metadata);

		/// <summary>
		/// Will find an existing font type with the same filename, or create a new font type.
		/// </summary>
		/// <param name="filename"> The family file path that the texture atlases and character infos are contained in. </param>
		/// <param name="style"> The style selected to load as this type. </param>
		static std::shared_ptr<FontType> Create(const std::string &filename, const std::string &style);

		/// <summary>
		/// Creates a new text loader.
		/// </summary>
		/// <param name="filename"> The family file path that the texture atlases and character infos are contained in. </param>
		/// <param name="fontStyle"> The style selected to load as this type. </param>
		FontType(const std::string &filename, const std::string &fontStyle);

		void Encode(Metadata &metadata) const override;

		const std::shared_ptr<Texture> &GetTexture() const { return m_texture; }

		const FontMetafile &GetMetadata() const { return m_metadata; }
	private:
		std::string m_filename;
		std::string m_style;

		std::shared_ptr<Texture> m_texture;
		FontMetafile m_metadata;
	};
}
