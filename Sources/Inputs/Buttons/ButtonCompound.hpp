#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Combines multiple button inputs into a single button.
 */
class ACID_EXPORT ButtonCompound : public Button::Registrar<ButtonCompound>, NonCopyable {
	inline static const bool Registered = Register("buttonCompound");
public:
	/**
	 * Creates a new compound button.
	 * @param buttons The buttons that will be combined into a compound button.
	 * @param useAnd If {@link ButtonCompound#IsDown} will check if all buttons are down instead of just one.
	 */
	explicit ButtonCompound(std::vector<std::unique_ptr<Button>> &&buttons = {}, bool useAnd = false);

	/**
	 * Creates a new compound button.
	 * @tparam Args The button argument types.
	 * @param args The buttons on the being added.
	 * @param useAnd If {@link ButtonCompound#IsDown} will check if all buttons are down instead of just one.
	 */
	template<typename... Args>
	ButtonCompound(bool useAnd, Args &&... args) :
		useAnd(useAnd) {
		buttons.reserve(sizeof...(Args));
		(buttons.emplace_back(std::forward<Args>(args)), ...);
		ConnectButtons();
	}

	/**
	 * Creates a new compound button.
	 * @tparam Args The button argument types.
	 * @param args The buttons on the being added.
	 */
	template<typename... Args>
	ButtonCompound(Args &&... args) {
		buttons.reserve(sizeof...(Args));
		(buttons.emplace_back(std::forward<Args>(args)), ...);
		ConnectButtons();
	}

	bool IsDown() const override;

	Axis::ArgumentDescription GetArgumentDescription() const override;

	const std::vector<std::unique_ptr<Button>> &GetButtons() const { return buttons; }
	Button *AddButton(std::unique_ptr<Button> &&button);
	void RemoveButton(Button *button);

	bool IsUseAnd() const { return useAnd; }
	void SetUseAnd(bool useAnd) { this->useAnd = useAnd; }

	friend const Node &operator>>(const Node &node, ButtonCompound &buttonCompound);
	friend Node &operator<<(Node &node, const ButtonCompound &buttonCompound);

private:
	void ConnectButton(std::unique_ptr<Button> &button);
	void ConnectButtons();

	std::vector<std::unique_ptr<Button>> buttons;
	bool useAnd = false;
	bool lastDown = false;
};
}
