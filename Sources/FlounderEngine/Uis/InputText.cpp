#include "InputText.hpp"

#include "../Devices/Keyboard.hpp"
#include "../Maths/Visual/DriverSlide.hpp"

namespace Flounder
{
	const float InputText::CHANGE_TIME = 0.1f;
	const float InputText::SCALE_NORMAL = 1.6f;
	const float InputText::SCALE_SELECTED = 1.8f;
	Colour *const InputText::COLOUR_NORMAL = new Colour("#000000");

	InputText::InputText(UiObject *parent, const Vector3 &position, const std::string &prefix, const std::string &value, const Justify &justify) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_text(nullptr),
		//new Text(this, position, SCALE_NORMAL, Vector2(0.5f, 0.5f), prefix + value, Uis::Get()->m_candara->GetRegular(), justify, 0.36f)),
		m_background(nullptr),
		//new Gui(this, position, Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f), new Texture("Resources/Guis/Button.png"), 1)),
		m_prefix(prefix),
		m_value(value),
		m_inputDelay(new InputDelay()),
		m_lastKey(0),
		m_selected(false),
		m_mouseOver(false),
		m_actionChange(nullptr)
	{
		m_background->SetColourOffset(Colour());
	}

	InputText::~InputText()
	{
		delete m_text;
		delete m_background;

		delete m_inputDelay;
	}

	void InputText::UpdateObject()
	{
		if (m_selected)
		{
			int key = Keyboard::Get()->GetChar();

			// TODO: Fix inputs that are not GLFW defined.
			if (key != 0 && Keyboard::Get()->GetKey(toupper(key)))
			{
				m_inputDelay->Update(true);

				if (m_lastKey != key || m_inputDelay->CanInput())
				{
					m_value += static_cast<char>(key);
					m_text->SetText(m_prefix + m_value);

					if (m_actionChange != 0)
					{
						m_actionChange();
					}

					m_lastKey = key;
				}
			}
			else if (Keyboard::Get()->GetKey(GLFW_KEY_BACKSPACE))
			{
				m_inputDelay->Update(true);

				if (m_lastKey != 8 || m_inputDelay->CanInput())
				{
					m_value = m_value.substr(0, m_value.length() - 1);
					m_text->SetText(m_prefix + m_value);

					if (m_actionChange != 0)
					{
						m_actionChange();
					}

					m_lastKey = 8;
				}
			}
			else if (Keyboard::Get()->GetKey(GLFW_KEY_ENTER) && m_lastKey != 13)
			{
				m_inputDelay->Update(true);

				m_selected = false;
				m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			}
			else
			{
				m_inputDelay->Update(false);
				m_lastKey = 0;
			}
		}

		// Click updates.
		if (Uis::Get()->GetSelector()->IsSelected(*m_text) && GetAlpha() == 1.0f &&
			Uis::Get()->GetSelector()->WasLeftClick())
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_selected = true;

			Uis::Get()->GetSelector()->CancelWasEvent();
		}
		else if (Uis::Get()->GetSelector()->WasLeftClick() && m_selected)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			m_selected = false;
		}

		// Mouse over updates.
		if (Uis::Get()->GetSelector()->IsSelected(*m_text) && !m_mouseOver && !m_selected)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector()->IsSelected(*m_text) && m_mouseOver && !m_selected)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			m_mouseOver = false;
		}

		// Update the background colour.
		Colour::Interpolate(*COLOUR_NORMAL, *Uis::Get()->GetManager()->GetPrimaryColour(),
			(m_text->GetScale() - SCALE_NORMAL) /
				(SCALE_SELECTED - SCALE_NORMAL), m_background->GetColourOffset());

		// Update background size.
		//m_background->GetDimensions()->Set(*m_text->GetDimensions());
		//m_background->GetDimensions()->m_y = 0.5f * static_cast<float>(m_text->GetFontType()->GetMetadata()->GetMaxSizeY());
		//Vector3::Multiply(*m_text->GetDimensions(), *m_background->GetDimensions(), m_background->GetDimensions());
		//m_background->GetDimensions()->Scale(2.0f * m_text->GetScale());
		//m_background->GetPosition()->Set(*m_text->GetPosition());
	}

	void InputText::SetPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		m_text->SetText(prefix + m_value);
	}

	void InputText::SetValue(const std::string &value)
	{
		m_value = value;
		m_text->SetText(m_prefix + value);
	}
}
