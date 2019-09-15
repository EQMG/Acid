#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Axis.hpp"

namespace acid {
/**
 * @brief Combines multiple axes inputs into a single axis.
 */
class ACID_EXPORT AxisCompound : public Axis, public NonCopyable {
public:
	/**
	 * Creates a new compound axis.
	 */
	AxisCompound() = default;

	/**
	 * Creates a new compound axis.
	 * @tparam Args The axis argument types.
	 * @param args The axes on the being added.
	 */
	template<typename... Args>
	AxisCompound(Args &&... args) {
		m_axes.reserve(sizeof...(Args));
		(m_axes.emplace_back(std::forward<Args>(args)), ...);
		ConnectAxes();
	}

	float GetAmount() const override;

private:
	void ConnectAxes();

	std::vector<std::unique_ptr<Axis>> m_axes;
};
}
