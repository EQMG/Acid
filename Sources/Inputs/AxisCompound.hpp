#pragma once

#include <memory>
#include <vector>
#include "Helpers/NonCopyable.hpp"
#include "Axis.hpp"

namespace acid
{
/**
 * @brief Combines multiple axes inputs into a single axis.
 */
class AxisCompound :
	public Axis,
	public NonCopyable
{
public:
	/**
	 * A template used to create a axis of a single type.
	 * @tparam T The type of buttons to create.
	 * @tparam Args The values passed to each axis.
	 * @param args The arguments to pass to T.
	 * @return The created compound axis.
	 */
	template<class T, typename... Args>
	static AxisCompound *Create(Args &&... args)
	{
		std::vector<Axis *> axes;

		for (const auto &x : { args... })
		{
			axes.emplace_back(new T(x));
		}

		return new AxisCompound(axes);
	}

	/**
	 * Creates a new compound axis.
	 * @param axes The axes on the being added.
	 */
	explicit AxisCompound(const std::vector<Axis *> &axes);

	float GetAmount() const override;

private:
	std::vector<std::unique_ptr<Axis>> m_axes;
};
}
