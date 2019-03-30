#pragma once

#include <memory>
#include <vector>
#include "Helpers/NonCopyable.hpp"
#include "IAxis.hpp"

namespace acid
{
/// <summary>
/// Axis composed of multiple other axes.
/// </summary>
class ACID_EXPORT AxisCompound :
	public IAxis, public NonCopyable
{
public:
	/// <summary>
	/// A template used to create a compound axis of a single type.
	/// </summary>
	/// <param name="args"> The arguments to pass to T. </param>
	/// <param name="T"> The type of axis to create. </param>
	/// <param name="Args"> The values passed to each axis. </param>
	template<class T, typename... Args>
	static AxisCompound *Create(Args &&... args)
	{
		std::vector<IAxis *> axes;

		for (const auto &x : { args... })
		{
			axes.emplace_back(new T(x));
		}

		return new AxisCompound(axes);
	}

	/// <summary>
	/// Creates a new compound axis.
	/// </summary>
	/// <param name="axes"> The axes on the being added. </param>
	explicit AxisCompound(const std::vector<IAxis *> &axes);

	float GetAmount() const override;

private:
	std::vector<std::unique_ptr<IAxis>> m_axes;
};
}
