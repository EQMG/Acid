#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Axis.hpp"

namespace acid {
/**
 * @brief Combines multiple axes inputs into a single axis.
 */
class ACID_EXPORT AxisCompound : public Axis::Registrar<AxisCompound>, NonCopyable {
public:
	/**
	 * Creates a new compound axis.
	 * @param axes The axes that will be combined into a compound axis.
	 */
	explicit AxisCompound(std::vector<std::unique_ptr<Axis>> &&axes = {});
	
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

	ArgumentDescription GetArgumentDescription() const override;

	const std::vector<std::unique_ptr<Axis>> &GetAxes() const { return m_axes; }
	Axis *AddAxis(std::unique_ptr<Axis> &&axis);
	void RemoveAxis(Axis *axis);

	friend const Node &operator>>(const Node &node, AxisCompound &axisCompound);
	friend Node &operator<<(Node &node, const AxisCompound &axisCompound);

private:
	void ConnectAxis(std::unique_ptr<Axis> &axis);
	void ConnectAxes();

	static bool registered;

	std::vector<std::unique_ptr<Axis>> m_axes;
};
}
