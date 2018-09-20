#pragma once

#include <memory>
#include <vector>
#include "IAxis.hpp"

namespace acid
{
	/// <summary>
	/// Axis composed of multiple other axes.
	/// </summary>
	class ACID_EXPORT AxisCompound :
		public IAxis
	{
	private:
		std::vector<std::unique_ptr<IAxis>> m_axes;
	public:
		/// <summary>
		/// Creates a new compound axis.
		/// </summary>
		/// <param name="axes"> The axes on the being added. </param>
		explicit AxisCompound(const std::vector<IAxis *> &axes);

		AxisCompound(const AxisCompound&) = delete;

		AxisCompound& operator=(const AxisCompound&) = delete;

		float GetAmount() const override;
	};
}
