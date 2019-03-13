#pragma once

#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiInputRadio : public UiObject
{
  public:
	enum class Mark
	{
		Filled,
		X,
		Dot,
		Check
	};

	UiInputRadio(UiObject* parent, const std::string& string, const Mark& markType = Mark::Filled, const bool& checked = false, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	void UpdateObject() override;

	const std::string& GetString() const
	{
		return m_text.GetString();
	}

	void SetString(const std::string& string)
	{
		m_text.SetString(string);
	}

	const bool& IsChecked() const
	{
		return m_checked;
	}

	void SetChecked(const bool& checked);

	const Mark& GetMarkType() const
	{
		return m_markType;
	}

	void SetMarkType(const Mark& markType);

	Delegate<void(UiInputRadio*, bool)>& GetOnChecked()
	{
		return m_onChecked;
	}

  private:
	void UpdateFill();

	Gui m_background;
	Gui m_fill;
	Text m_text;
	Sound m_soundClick;

	bool m_checked;
	Mark m_markType;
	bool m_mouseOver;

	Delegate<void(UiInputRadio*, bool)> m_onChecked;
};

class ACID_EXPORT UiRadioManager : public NonCopyable
{
  public:
	explicit UiRadioManager(const UiInputRadio::Mark& markType = UiInputRadio::Mark::X, const bool& multiple = false, const std::vector<UiInputRadio*>& inputs = {}) : m_markType(markType), m_multiple(multiple), m_inputs(inputs)
	{
		for(auto& input : inputs)
			{
				input->SetMarkType(markType);
				input->GetOnChecked() += [this](UiInputRadio* object, bool value) {
					if(!m_multiple)
						{
							for(auto& input2 : m_inputs)
								{
									if(input2 != object)
										{
											input2->SetChecked(false);
										}
								}
						}
				};
			}
	}

	const UiInputRadio::Mark& GetMarkType() const
	{
		return m_markType;
	}

	void SetMarkType(const UiInputRadio::Mark& markType)
	{
		m_markType = markType;

		for(auto& input : m_inputs)
			{
				input->SetMarkType(m_markType);
			}
	}

	const std::vector<UiInputRadio*>& GetInputs() const
	{
		return m_inputs;
	}

	void AddInput(UiInputRadio* input)
	{
		m_inputs.emplace_back(input);
	}

	void RemoveInput(UiInputRadio* input)
	{
		m_inputs.erase(std::remove(m_inputs.begin(), m_inputs.end(), input), m_inputs.end());
	}

	void ClearInputs()
	{
		m_inputs.clear();
	}

  private:
	UiInputRadio::Mark m_markType;
	bool m_multiple;
	std::vector<UiInputRadio*> m_inputs;
};
}
