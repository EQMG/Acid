#include "Keyboard.hpp"

#include <GLFW/glfw3.h>

namespace acid
{
void CallbackKey(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
	Keyboard::Get()->m_onKey(static_cast<Key>(key), static_cast<InputAction>(action), MakeBitMask<InputMod>(mods));
}

void CallbackChar(GLFWwindow* window, uint32_t codepoint)
{
	Keyboard::Get()->m_onChar(static_cast<char>(codepoint));
}

Keyboard::Keyboard()
{
	glfwSetKeyCallback(Window::Get()->GetWindow(), CallbackKey);
	glfwSetCharCallback(Window::Get()->GetWindow(), CallbackChar);
}

void Keyboard::Update() {}

InputAction Keyboard::GetKey(const Key& key) const
{
	auto state = glfwGetKey(Window::Get()->GetWindow(), static_cast<int32_t>(key));
	return static_cast<InputAction>(state);
}

std::string Keyboard::ToString(const Key& key)
{
	switch(key)
		{
			case Key::Space:
				return "Space";
			case Key::Apostrophe:
				return "Apostrophe";
			case Key::Comma:
				return "Comma";
			case Key::Minus:
				return "Minus";
			case Key::Period:
				return "Period";
			case Key::Slash:
				return "Slash";
			case Key::_0:
				return "0";
			case Key::_1:
				return "1";
			case Key::_2:
				return "2";
			case Key::_3:
				return "3";
			case Key::_4:
				return "4";
			case Key::_5:
				return "5";
			case Key::_6:
				return "6";
			case Key::_7:
				return "7";
			case Key::_8:
				return "8";
			case Key::_9:
				return "9";
			case Key::Semicolon:
				return "Semicolon";
			case Key::Equal:
				return "Equal";
			case Key::A:
				return "A";
			case Key::B:
				return "B";
			case Key::C:
				return "C";
			case Key::D:
				return "D";
			case Key::E:
				return "E";
			case Key::F:
				return "F";
			case Key::G:
				return "G";
			case Key::H:
				return "H";
			case Key::I:
				return "I";
			case Key::J:
				return "J";
			case Key::K:
				return "K";
			case Key::L:
				return "L";
			case Key::M:
				return "M";
			case Key::N:
				return "N";
			case Key::O:
				return "O";
			case Key::P:
				return "P";
			case Key::Q:
				return "Q";
			case Key::R:
				return "R";
			case Key::S:
				return "S";
			case Key::T:
				return "T";
			case Key::U:
				return "U";
			case Key::V:
				return "V";
			case Key::W:
				return "W";
			case Key::X:
				return "X";
			case Key::Y:
				return "Y";
			case Key::Z:
				return "Z";
			case Key::LeftBracket:
				return "Left Bracket";
			case Key::Backslash:
				return "Backslash";
			case Key::RightBracket:
				return "Right Bracket";
			case Key::GraveAccent:
				return "Grave Accent";
			case Key::World1:
				return "World1";
			case Key::World2:
				return "World2";
			case Key::Escape:
				return "Escape";
			case Key::Enter:
				return "Enter";
			case Key::Tab:
				return "Tab";
			case Key::Backspace:
				return "Backspace";
			case Key::Insert:
				return "Insert";
			case Key::Delete:
				return "Delete";
			case Key::Right:
				return "Right";
			case Key::Left:
				return "Left";
			case Key::Down:
				return "Down";
			case Key::Up:
				return "Up";
			case Key::PageUp:
				return "Page Up";
			case Key::PageDown:
				return "Page Down";
			case Key::Home:
				return "Home";
			case Key::End:
				return "End";
			case Key::CapsLock:
				return "Caps Lock";
			case Key::ScrollLock:
				return "Scroll Lock";
			case Key::NumLock:
				return "Num Lock";
			case Key::PrintScreen:
				return "Print Screen";
			case Key::Pause:
				return "Pause";
			case Key::F1:
				return "F1";
			case Key::F2:
				return "F2";
			case Key::F3:
				return "F3";
			case Key::F4:
				return "F4";
			case Key::F5:
				return "F5";
			case Key::F6:
				return "F6";
			case Key::F7:
				return "F7";
			case Key::F8:
				return "F8";
			case Key::F9:
				return "F9";
			case Key::F10:
				return "F10";
			case Key::F11:
				return "F11";
			case Key::F12:
				return "F12";
			case Key::F13:
				return "F13";
			case Key::F14:
				return "F14";
			case Key::F15:
				return "F15";
			case Key::F16:
				return "F16";
			case Key::F17:
				return "F17";
			case Key::F18:
				return "F18";
			case Key::F19:
				return "F19";
			case Key::F20:
				return "F20";
			case Key::F21:
				return "F21";
			case Key::F22:
				return "F22";
			case Key::F23:
				return "F23";
			case Key::F24:
				return "F24";
			case Key::F25:
				return "F25";
			case Key::Numpad0:
				return "Numpad 0";
			case Key::Numpad1:
				return "Numpad 1";
			case Key::Numpad2:
				return "Numpad 2";
			case Key::Numpad3:
				return "Numpad 3";
			case Key::Numpad4:
				return "Numpad 4";
			case Key::Numpad5:
				return "Numpad 5";
			case Key::Numpad6:
				return "Numpad 6";
			case Key::Numpad7:
				return "Numpad 7";
			case Key::Numpad8:
				return "Numpad 8";
			case Key::Numpad9:
				return "Numpad 9";
			case Key::NumpadDecimal:
				return "Numpad Decimal";
			case Key::NumpadDivide:
				return "Numpad Divide";
			case Key::NumpadMultiply:
				return "Numpad Multiply";
			case Key::NumpadSubtract:
				return "Numpad Subtract";
			case Key::NumpadAdd:
				return "Numpad Add";
			case Key::NumpadEnter:
				return "Numpad Enter";
			case Key::NumpadEqual:
				return "Numpad Equal";
			case Key::ShiftLeft:
				return "Shift Left";
			case Key::ControlLeft:
				return "Control Left";
			case Key::AltLeft:
				return "Alt Left";
			case Key::SuperLeft:
				return "Super Left";
			case Key::ShiftRight:
				return "Shift Right";
			case Key::ControlRight:
				return "Control Right";
			case Key::AltRight:
				return "Alt Right";
			case Key::SuperRight:
				return "Super Right";
			case Key::Menu:
				return "Menu";
			default:
				return "Undefined";
		}
}
}
