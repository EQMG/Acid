#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Button.hpp"

namespace acid
{
/**
 * @brief Combines multiple button inputs into a single button.
 */
class ACID_EXPORT ButtonCompound :
	public Button,
	public NonCopyable
{
public:
	/**
	 * Creates a new compound button.
	 * @param buttons The buttons on the being added.
	 * @param useAnd If {@link ButtonCompound#IsDown} will check if all buttons are down instead of just one.
	 */
	explicit ButtonCompound(std::vector<std::unique_ptr<Button>> &&buttons, const bool &useAnd = false);

	template <typename... Args>
	explicit ButtonCompound(Args &&... args)
	{
		m_buttons.reserve(sizeof...(Args));
		(m_buttons.emplace_back(std::forward<Args>(args)), ...);
	}

	bool IsDown() const override;

private:
	std::vector<std::unique_ptr<Button>> m_buttons;
	bool m_useAnd{};
	bool m_lastDown{};
};
}
