#pragma once

#include <string>
#include "Resources/Resources.hpp"
#include "FontType.hpp"

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

	class FL_EXPORT FontFamily :
		public IResource
	{
	private:
		std::string m_filename;

		std::shared_ptr<FontType> m_typeThin;
		std::shared_ptr<FontType> m_typeLight;
		std::shared_ptr<FontType> m_typeRegular;
		std::shared_ptr<FontType> m_typeSemibold;
		std::shared_ptr<FontType> m_typeBold;
	public:
		static std::shared_ptr<FontFamily> Resource(const std::string &filename)
		{
			auto resource = Resources::Get()->Get(filename);

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<FontFamily>(resource);
			}

			auto result = std::make_shared<FontFamily>(filename);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<FontType> Resource(const std::string &filename, const FamilyType &familyType)
		{
			return Resource(filename)->GetType(familyType);
		}

		FontFamily(const std::string &filename);

		~FontFamily();

		std::string GetFilename() override { return m_filename; }

		std::shared_ptr<FontType> GetType(const FamilyType &familyType);

		std::shared_ptr<FontType> GetThin() const { return m_typeThin; }

		std::shared_ptr<FontType> GetLight() const { return m_typeLight; }

		std::shared_ptr<FontType> GetRegular() const { return m_typeRegular; }

		std::shared_ptr<FontType> GetSemibold() const { return m_typeSemibold; }

		std::shared_ptr<FontType> GetBold() const { return m_typeBold; }
	private:
		static std::shared_ptr<FontType> LoadFontType(const std::string &filename, const std::string &suffex);
	};
}
