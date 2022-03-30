#include "Windows.hpp"

#include <algorithm>
#include <GLFW/glfw3.h>

#include "Graphics/Graphics.hpp"

namespace acid {
static_assert(GLFW_KEY_LAST == static_cast<int16_t>(Key::Menu), "GLFW keys count does not match our keys enum count.");
static_assert(GLFW_MOUSE_BUTTON_LAST == static_cast<int16_t>(MouseButton::_8), "GLFW mouse button count does not match our mouse button enum count.");

void CallbackWindowPosition(GLFWwindow *glfwWindow, int32_t xpos, int32_t ypos) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	if (window->fullscreen) return;

	window->position = {xpos, ypos};
	window->onPosition(window->position);
}

void CallbackWindowSize(GLFWwindow *glfwWindow, int32_t width, int32_t height) {
	if (width <= 0 || height <= 0) return;
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));

	if (window->fullscreen) {
		window->fullscreenSize = {width, height};
		window->onSize(window->fullscreenSize);
	} else {
		window->size = {width, height};
		window->onSize(window->size);
	}
}

void CallbackWindowClose(GLFWwindow *glfwWindow) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	window->closed = false;
	//Engine::Get()->RequestClose();
	window->onClose();
}

void CallbackWindowFocus(GLFWwindow *glfwWindow, int32_t focused) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	window->focused = static_cast<bool>(focused);
	window->onFocus(focused == GLFW_TRUE);
}

void CallbackWindowIconify(GLFWwindow *glfwWindow, int32_t iconified) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	window->iconified = iconified;
	window->onIconify(iconified);
}

void CallbackFramebufferSize(GLFWwindow *glfwWindow, int32_t width, int32_t height) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	if (window->fullscreen)
		window->fullscreenSize = {width, height};
	else
		window->size = {width, height};
	// TODO: only set this window to resized
	Graphics::Get()->SetFramebufferResized(window->id);
}

void CallbackCursorEnter(GLFWwindow *glfwWindow, int32_t entered) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	window->windowSelected = entered == GLFW_TRUE;
	window->onEnter(entered == GLFW_TRUE);
}

void CallbackDrop(GLFWwindow *glfwWindow, int32_t count, const char **paths) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	std::vector<std::string> files(static_cast<uint32_t>(count));
	for (uint32_t i = 0; i < static_cast<uint32_t>(count); i++)
		files[i] = paths[i];

	window->onDrop(files);
}

void CallbackKey(GLFWwindow *glfwWindow, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	window->onKey(static_cast<Key>(key), static_cast<InputAction>(action), bitmask::bitmask<InputMod>(mods));
}

void CallbackChar(GLFWwindow *glfwWindow, uint32_t codepoint) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	window->onChar(static_cast<char>(codepoint));
}

void CallbackMouseButton(GLFWwindow *glfwWindow, int32_t button, int32_t action, int32_t mods) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	window->onMouseButton(static_cast<MouseButton>(button), static_cast<InputAction>(action), bitmask::bitmask<InputMod>(mods));
}

void CallbackCursorPos(GLFWwindow *glfwWindow, double xpos, double ypos) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	window->mousePosition = {xpos, ypos};
	window->onMousePosition(window->mousePosition);
}

void CallbackScroll(GLFWwindow *glfwWindow, double xoffset, double yoffset) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	window->mouseScroll = {yoffset, yoffset};
	window->onMouseScroll(window->mouseScroll);
}

double Window::SmoothScrollWheel(double value, float delta) {
	if (value != 0.0) {
		value -= static_cast<double>(delta) * std::copysign(3.0, value);
		value = Maths::Deadband(0.08, value);
		return value;
	}

	return 0.0;
}

Window::Window(std::size_t id) :
	id(id),
	size(1080, 720),
	title("Acid Window"),
	resizable(true),
	focused(true) {
	// Create a windowed mode window and its context.
	window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

	// Gets any window errors.
	if (!window) {
		glfwTerminate();
		throw std::runtime_error("GLFW failed to create the window");
	}

	// Sets the user pointer.
	glfwSetWindowUserPointer(window, this);

	// Window attributes that can change later.
	glfwSetWindowAttrib(window, GLFW_DECORATED, !borderless);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, resizable);
	glfwSetWindowAttrib(window, GLFW_FLOATING, floating);

	// Centre the window position.
	//position.x = (videoMode.width - size.x) / 2;
	//position.y = (videoMode.height - size.y) / 2;
	//glfwSetWindowPos(window, position.x, position.y);

	// Sets fullscreen if enabled.
	if (fullscreen)
		SetFullscreen(true);

	// Shows the glfw window.
	glfwShowWindow(window);

	// Sets the displays callbacks.
	glfwSetWindowPosCallback(window, CallbackWindowPosition);
	glfwSetWindowSizeCallback(window, CallbackWindowSize);
	glfwSetWindowCloseCallback(window, CallbackWindowClose);
	glfwSetWindowFocusCallback(window, CallbackWindowFocus);
	glfwSetWindowIconifyCallback(window, CallbackWindowIconify);
	glfwSetFramebufferSizeCallback(window, CallbackFramebufferSize);
	glfwSetCursorEnterCallback(window, CallbackCursorEnter);
	glfwSetDropCallback(window, CallbackDrop);
	glfwSetKeyCallback(window, CallbackKey);
	glfwSetCharCallback(window, CallbackChar);
	glfwSetMouseButtonCallback(window, CallbackMouseButton);
	glfwSetCursorPosCallback(window, CallbackCursorPos);
	glfwSetScrollCallback(window, CallbackScroll);
}

Window::~Window() {
	// Free the window callbacks and destroy the window.
	glfwDestroyWindow(window);

	closed = true;
}

void Window::Update() {
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	// Updates the position delta.
	mousePositionDelta = delta * (mouseLastPosition - mousePosition);
	mouseLastPosition = mousePosition;

	// Updates the scroll delta.
	mouseScrollDelta = delta * (mouseLastScroll - mouseScroll);
	mouseLastScroll = mouseScroll;
}

void Window::SetSize(const Vector2i &size) {
	if (size.x != -1)
		this->size.x = size.x;
	if (size.y != -1)
		this->size.y = size.y;
	glfwSetWindowSize(window, size.x, size.y);
}

void Window::SetPosition(const Vector2i &position) {
	if (position.x != -1)
		this->position.x = position.x;
	if (position.x != -1)
		this->position.y = position.y;
	glfwSetWindowPos(window, position.x, position.y);
}

void Window::SetTitle(const std::string &title) {
	this->title = title;
	glfwSetWindowTitle(window, title.c_str());
	onTitle(title);
}

void Window::SetIcons(const std::vector<std::filesystem::path> &filenames) {
	std::vector<std::unique_ptr<Bitmap>> bitmaps;
	std::vector<GLFWimage> icons;

	for (const auto &filename : filenames) {
		auto bitmap = std::make_unique<Bitmap>(filename);

		if (!bitmap) continue;

		GLFWimage icon = {};
		icon.width = bitmap->GetSize().x;
		icon.height = bitmap->GetSize().y;
		icon.pixels = bitmap->GetData().get();
		icons.emplace_back(icon);
		bitmaps.emplace_back(std::move(bitmap));
	}

	glfwSetWindowIcon(window, static_cast<int32_t>(icons.size()), icons.data());
}

void Window::SetBorderless(bool borderless) {
	this->borderless = borderless;
	glfwSetWindowAttrib(window, GLFW_DECORATED, !borderless);
	onBorderless(borderless);
}

void Window::SetResizable(bool resizable) {
	this->resizable = resizable;
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, resizable);
	onResizable(resizable);
}

void Window::SetFloating(bool floating) {
	this->floating = floating;
	glfwSetWindowAttrib(window, GLFW_FLOATING, floating);
	onFloating(floating);
}

void Window::SetFullscreen(bool fullscreen, Monitor *monitor) {
	auto selected = monitor ? monitor : GetCurrentMonitor();
	auto videoMode = selected->GetVideoMode();

	this->fullscreen = fullscreen;

	if (fullscreen) {
#ifdef ACID_DEBUG
		Log::Out("Window is going fullscreen\n");
#endif
		fullscreenSize = {videoMode.width, videoMode.height};
		glfwSetWindowMonitor(window, selected->GetMonitor(), 0, 0, fullscreenSize.x, fullscreenSize.y, GLFW_DONT_CARE);
	} else {
#ifdef ACID_DEBUG
		Log::Out("Window is going windowed\n");
#endif
		position = ((Vector2i(videoMode.width, videoMode.height) - size) / 2) + selected->GetPosition();
		glfwSetWindowMonitor(window, nullptr, position.x, position.y, size.x, size.y, GLFW_DONT_CARE);
	}

	onFullscreen(fullscreen);
}

void Window::SetIconified(bool iconify) {
	if (!iconified && iconify) {
		glfwIconifyWindow(window);
	} else if (iconified && !iconify) {
		glfwRestoreWindow(window);
	}
}

std::string Window::GetClipboard() const {
	return glfwGetClipboardString(window);
}

void Window::SetClipboard(const std::string &string) const {
	glfwSetClipboardString(window, string.c_str());
}

void Window::SetCursorHidden(bool hidden) {
	if (cursorHidden != hidden) {
		glfwSetInputMode(window, GLFW_CURSOR, hidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

		if (!hidden && cursorHidden)
			SetMousePosition(mousePosition);
	}

	cursorHidden = hidden;
}

void Window::SetCursor(const Cursor *cursor) {
	glfwSetCursor(window, cursor ? cursor->cursor : nullptr);
}


InputAction Window::GetKey(Key key) const {
	auto state = glfwGetKey(window, static_cast<int32_t>(key));
	return static_cast<InputAction>(state);
}

InputAction Window::GetMouseButton(MouseButton mouseButton) const {
	auto state = glfwGetMouseButton(window, static_cast<int32_t>(mouseButton));
	return static_cast<InputAction>(state);
}

void Window::SetMousePosition(const Vector2d &mousePosition) {
	this->mouseLastPosition = mousePosition;
	this->mousePosition = mousePosition;
	glfwSetCursorPos(window, mousePosition.x, mousePosition.y);
}

void Window::SetMouseScroll(const Vector2d &mouseScroll) {
	this->mouseLastScroll = mouseScroll;
	this->mouseScroll = mouseScroll;
}

std::string Window::ToString(Key key) {
	switch (key) {
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

int32_t OverlappingArea(Vector2i l1, Vector2i r1, Vector2i l2, Vector2i r2) {
	int area1 = std::abs(l1.x - r1.x) * std::abs(l1.y - r1.y);

	int area2 = std::abs(l2.x - r2.x) * std::abs(l2.y - r2.y);

	int areaI = (std::min(r1.x, r2.x) - std::max(l1.x, l2.x)) *
		(std::min(r1.y, r2.y) - std::max(l1.y, l2.y));

	return area1 + area2 - areaI;
}

const Monitor *Window::GetCurrentMonitor() const {
	if (fullscreen) {
		auto glfwMonitor = glfwGetWindowMonitor(window);
		for (const auto &monitor : Windows::Get()->GetMonitors()) {
			if (monitor->monitor == glfwMonitor)
				return monitor.get();
		}
		return nullptr;
	}
	std::multimap<int32_t, const Monitor *> rankedMonitor;
	auto where = rankedMonitor.end();

	for (const auto &monitor : Windows::Get()->GetMonitors()) {
		where = rankedMonitor.insert(where, {OverlappingArea(monitor->GetWorkareaPosition(), monitor->GetWorkareaPosition() + monitor->GetWorkareaSize(),
			position, position + size), monitor.get()});
	}

	if (rankedMonitor.begin()->first > 0)
		return rankedMonitor.begin()->second;

	return nullptr;
}

VkResult Window::CreateSurface(const VkInstance &instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) const {
	return glfwCreateWindowSurface(instance, window, allocator, surface);
}
}
