#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Combines multiple button inputs into a single button.
 */
class ACID_EXPORT ButtonCompound : public Button::Registrar<ButtonCompound>, public virtual NonCopyable {
public:
	/**
	 * Creates a new compound button.
	 */
	ButtonCompound() = default;

	/**
	 * Creates a new compound button.
	 * @param buttons The buttons that will be combined into a compound button.
	 * @param useAnd If {@link ButtonCompound#IsDown} will check if all buttons are down instead of just one.
	 */
	ButtonCompound(std::vector<std::unique_ptr<Button>> &&buttons, bool useAnd = false);

	/**
	 * Creates a new compound button.
	 * @tparam Args The button argument types.
	 * @param args The buttons on the being added.
	 * @param useAnd If {@link ButtonCompound#IsDown} will check if all buttons are down instead of just one.
	 */
	template<typename... Args>
	ButtonCompound(bool useAnd, Args &&... args) :
		m_useAnd(useAnd) {
		m_buttons.reserve(sizeof...(Args));
		(m_buttons.emplace_back(std::forward<Args>(args)), ...);
		ConnectButtons();
	}

	/**
	 * Creates a new compound button.
	 * @tparam Args The button argument types.
	 * @param args The buttons on the being added.
	 */
	template<typename... Args>
	ButtonCompound(Args &&... args) {
		m_buttons.reserve(sizeof...(Args));
		(m_buttons.emplace_back(std::forward<Args>(args)), ...);
		ConnectButtons();
	}

	bool IsDown() const override;

	Axis::ArgumentDescription GetArgumentDescription() const override;

	const std::vector<std::unique_ptr<Button>> &GetButtons() const { return m_buttons; }

	bool IsUseAnd() const { return m_useAnd; }
	void SetUseAnd(bool useAnd) { m_useAnd = useAnd; }

	friend const Node &operator>>(const Node &node, ButtonCompound &buttonCompound);
	friend Node &operator<<(Node &node, const ButtonCompound &buttonCompound);

private:
	void ConnectButtons();

	static bool registered;

	std::vector<std::unique_ptr<Button>> m_buttons;
	bool m_useAnd = false;
	bool m_lastDown = false;
};
}
