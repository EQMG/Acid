#include "FontType.hpp"
#include <utility>

#include "Resources/Resources.hpp"

namespace acid
{
	std::shared_ptr<FontType> FontType::Create(const Metadata &metadata)
	{
		auto resource = Resources::Get()->Find(metadata);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<FontType>(resource);
		}

		auto result = std::make_shared<FontType>("", "");
		Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<FontType> FontType::Create(const std::string &filename, const std::string &style)
	{
		auto temp = FontType(filename, style, false);
		temp.m_filename = filename;
		temp.m_style = style;
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	FontType::FontType(std::string filename, std::string style, const bool &load) :
		m_filename(std::move(filename)),
		m_style(std::move(style)),
		m_texture(nullptr),
		m_metadata(nullptr)
	{
		if (load)
		{
			FontType::Load();
		}
	}

	void FontType::Load()
	{
		if (m_filename.empty() || m_style.empty())
		{
			return;
		}

		m_texture = Texture::Create(m_filename + "/" + m_style + ".png");
		m_metadata = std::make_unique<FontMetafile>(m_filename + "/" + m_style + ".fnt");
	}

	void FontType::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Filename", m_filename);
		metadata.GetChild("Style", m_style);
	}

	void FontType::Encode(Metadata &metadata) const
	{
		metadata.SetChild("Filename", m_filename);
		metadata.SetChild("Style", m_style);
	}
}
