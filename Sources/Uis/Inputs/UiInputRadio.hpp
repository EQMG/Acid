#pragma once

#include <functional>
#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
	class ACID_EXPORT UiInputRadio :
		public UiObject
	{
	public:
		enum class Mark
		{
			Filled,
			X,
			Dot,
			Check
		};

		UiInputRadio(UiObject *parent, const std::string &string, const Mark &markType = Mark::Filled, const bool &checked = false,
			const UiBound &rectangle = UiBound(Vector3::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

		void UpdateObject() override;

		const std::string &GetString() const { return m_text->GetString(); }

		void SetString(const std::string &string) { m_text->SetString(string); }

		const bool &IsChecked() const { return m_checked; }

		void SetChecked(const bool &checked);

		const Mark &GetMarkType() const { return m_markType; }

		void SetMarkType(const Mark &markType);

		Delegate<void(UiInputRadio *, bool)> &GetOnChecked() { return m_onChecked; }
	private:
		void UpdateFill();

		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Gui> m_fill;
		std::unique_ptr<Text> m_text;
		Sound m_soundClick;

		bool m_checked;
		Mark m_markType;
		bool m_mouseOver;

		Delegate<void(UiInputRadio *, bool)> m_onChecked;
	};

	class ACID_EXPORT UiRadioManager :
		public NonCopyable
	{
	public:
		explicit UiRadioManager(const UiInputRadio::Mark &markType = UiInputRadio::Mark::X, const bool &multiple = false,
			const std::vector<UiInputRadio *> &inputs = {}) :
			m_markType(markType),
			m_multiple(multiple),
			m_inputs(std::vector<std::unique_ptr<UiInputRadio>>())
		{
			for (auto &input : inputs)
			{
				input->SetMarkType(markType);
				input->GetOnChecked() += [this](UiInputRadio *object, bool value) {
					if (!m_multiple)
					{
						for (auto &input2 : m_inputs)
						{
							if (input2.get() != object)
							{
								input2->SetChecked(false);
							}
						}
					}
				};
				m_inputs.emplace_back(input);
			}
		}

		const UiInputRadio::Mark &GetMarkType() const { return m_markType; }

		void SetMarkType(const UiInputRadio::Mark &markType)
		{
			m_markType = markType;

			for (auto &input : m_inputs)
			{
				input->SetMarkType(m_markType);
			}
		}

		const std::vector<std::unique_ptr<UiInputRadio>> &GetInputs() const { return m_inputs; }
	private:
		UiInputRadio::Mark m_markType;
		bool m_multiple;
		std::vector<std::unique_ptr<UiInputRadio>> m_inputs;
	};
}
