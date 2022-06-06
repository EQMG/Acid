#pragma once

#include "Utils/NonCopyable.hpp"
#include "Inputs/InputButton.hpp"

namespace acid {
/**
 * @brief Combines multiple button inputs into a single button.
 */
class ACID_CONTEXT_EXPORT CompoundInputButton : public InputButton::Registrar<CompoundInputButton>, NonCopyable {
	inline static const bool Registered = Register("compound");
public:
	/**
	 * Creates a new compound button.
	 * @param buttons The buttons that will be combined into a compound button.
	 * @param useAnd If {@link CompoundInputButton#IsDown} will check if all buttons are down instead of just one.
	 */
	explicit CompoundInputButton(std::vector<std::unique_ptr<InputButton>> &&buttons = {}, bool useAnd = false);

	/**
	 * Creates a new compound button.
	 * @tparam Args The button argument types.
	 * @param args The buttons on the being added.
	 * @param useAnd If {@link CompoundInputButton#IsDown} will check if all buttons are down instead of just one.
	 */
	template<typename... Args>
	CompoundInputButton(bool useAnd, Args &&... args) :
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
	CompoundInputButton(Args &&... args) {
		buttons.reserve(sizeof...(Args));
		(buttons.emplace_back(std::forward<Args>(args)), ...);
		ConnectButtons();
	}

	bool IsDown() const override;

	InputAxis::ArgumentDescription GetArgumentDescription() const override;

	const std::vector<std::unique_ptr<InputButton>> &GetButtons() const { return buttons; }
	InputButton *AddButton(std::unique_ptr<InputButton> &&button);
	void RemoveButton(InputButton *button);

	bool IsUseAnd() const { return useAnd; }
	void SetUseAnd(bool useAnd) { this->useAnd = useAnd; }

	friend const Node &operator>>(const Node &node, CompoundInputButton &inputButton);
	friend Node &operator<<(Node &node, const CompoundInputButton &inputButton);

private:
	void ConnectButton(std::unique_ptr<InputButton> &button);
	void ConnectButtons();

	std::vector<std::unique_ptr<InputButton>> buttons;
	bool useAnd = false;
	bool lastDown = false;
};
}
