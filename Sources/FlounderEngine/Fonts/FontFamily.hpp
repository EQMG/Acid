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
		static FontFamily *Resource(const std::string &filename)
		{
			IResource *resource = Resources::Get()->Get(filename);

			if (resource != nullptr)
			{
				return dynamic_cast<FontFamily *>(resource);
			}

			FontFamily *result = new FontFamily(filename);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
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
