#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Button.hpp"

namespace acid
{
/**
 * @brief Combines multiple button inputs into a single button.
 */
class ButtonCompound :
	public Button,
	public NonCopyable
{
public:
	/**
	 * A template used to create a compound button of a single type.
	 * @tparam T The type of buttons to create.
	 * @tparam Args The values passed to each button.
	 * @param useAnd If {@link ButtonCompound#IsDown} will check if all buttons are down instead of just one.
	 * @param args The arguments to pass to T.
	 * @return The created compound button.
	 */
	template<class T, typename... Args>
	static ButtonCompound *Create(const bool &useAnd, Args &&... args)
	{
		std::vector<Button *> buttons;

		for (const auto &x : { args... })
		{
			buttons.emplace_back(new T(x));
		}

		return new ButtonCompound(buttons, useAnd);
	}

	/**
	 * Creates a new compound button.
	 * @param buttons The buttons on the being added.
	 * @param useAnd If {@link ButtonCompound#IsDown} will check if all buttons are down instead of just one.
	 */
	explicit ButtonCompound(const std::vector<Button *> &buttons, const bool &useAnd = false);

	bool IsDown() const override;

private:
	std::vector<std::unique_ptr<Button>> m_buttons;
	bool m_useAnd;
	bool m_lastDown;
};
}
