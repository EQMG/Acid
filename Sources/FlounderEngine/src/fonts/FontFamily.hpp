#pragma once

#include <string>
#include "FontType.hpp"

namespace Flounder
{
	class FontFamily
	{
	private:
		std::string m_name;

		FontType *m_typeThin;
		FontType *m_typeLight;
		FontType *m_typeRegular;
		FontType *m_typeSemibold;
		FontType *m_typeBold;
	public:
		FontFamily(const std::string &name);

		~FontFamily();

		std::string GetName() const { return m_name; }

		FontType *GetThin() const { return m_typeThin; }

		FontType *GetLight() const { return m_typeLight; }

		FontType *GetRegular() const { return m_typeRegular; }

		FontType *GetSemibold() const { return m_typeSemibold; }

		FontType *GetBold() const { return m_typeBold; }
	private:
		static FontType *LoadFontType(const std::string &name, const std::string &type);
	};
}
