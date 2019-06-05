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
class ACID_EXPORT AxisCompound :
	public Axis,
	public NonCopyable
{
public:
	/**
	 * Creates a new compound axis.
	 * @param axes The axes on the being added.
	 */
	explicit AxisCompound(std::vector<std::unique_ptr<Axis>> &&axes);

	template <typename... Args>
	explicit AxisCompound(Args &&... args)
	{
		/*m_axes.reserve(sizeof...(Args));

		for (auto &&arg : {args...})
		{
			m_axes.emplace_back(std::move(arg));
		}*/
	}

	float GetAmount() const override;

private:
	std::vector<std::unique_ptr<Axis>> m_axes;
};
}
