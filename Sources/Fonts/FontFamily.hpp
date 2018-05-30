#pragma once

#include <string>
#include "Resources/Resources.hpp"
#include "FontType.hpp"

namespace fl
{
	class FL_EXPORT FontFamily :
		public IResource
	{
	private:
		std::string m_filename;

		FontType *m_typeThin;
		FontType *m_typeLight;
		FontType *m_typeRegular;
		FontType *m_typeSemibold;
		FontType *m_typeBold;
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

		FontFamily(const std::string &filename);

		~FontFamily();

		std::string GetFilename() override { return m_filename; }

		FontType *GetThin() const { return m_typeThin; }

		FontType *GetLight() const { return m_typeLight; }

		FontType *GetRegular() const { return m_typeRegular; }

		FontType *GetSemibold() const { return m_typeSemibold; }

		FontType *GetBold() const { return m_typeBold; }
	private:
		static FontType *LoadFontType(const std::string &filename, const std::string &suffex);
	};
}
