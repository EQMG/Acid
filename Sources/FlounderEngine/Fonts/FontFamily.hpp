#pragma once

#include <string>
#include "FontType.hpp"

namespace Flounder
{
	class FontFamily
	{
	private:
		std::string m_filename;

		FontType *m_typeThin;
		FontType *m_typeLight;
		FontType *m_typeRegular;
		FontType *m_typeSemibold;
		FontType *m_typeBold;
	public:
		FontFamily(const std::string &filename);

		~FontFamily();

		std::string GetFilename() const { return m_filename; }

		FontType *GetThin() const { return m_typeThin; }

		FontType *GetLight() const { return m_typeLight; }

		FontType *GetRegular() const { return m_typeRegular; }

		FontType *GetSemibold() const { return m_typeSemibold; }

		FontType *GetBold() const { return m_typeBold; }
	private:
		static FontType *LoadFontType(const std::string &filename, const std::string &suffex);
	};
}
