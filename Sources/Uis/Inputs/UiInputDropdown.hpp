#pragma once

#include <functional>
#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiInputDropdown :
	public UiObject
{
public:
	UiInputDropdown(UiObject *parent, const std::string &title, const uint32_t &value, const std::vector<std::string> &options,
		const UiBound &rectangle = UiBound(Vector2f(0.0f, 0.0f), UiReference::Centre, UiAspect::Position | UiAspect::Size));

	void UpdateObject() override;
	
	const std::string &GetTitle() const { return m_textTitle.GetString(); }

	void SetTitle(const std::string &title) { m_textTitle.SetString(title); }

private:
	Gui m_background;
	Text m_textTitle;
	Sound m_soundClick;
};
}
