#pragma once

#include <vector>
#include "IAxis.hpp"

namespace fl
{
	/// <summary>
	/// Axis composed of multiple other axes.
	/// </summary>
	class FL_EXPORT AxisCompound :
		public IAxis
	{
	private:
		std::vector<IAxis *> m_axes;
	public:
		/// <summary>
		/// Creates a new compound axis.
		/// </summary>
		/// <param name="axes"> The axes on the being added. </param>
		AxisCompound(const std::vector<IAxis *> &axes);

		/// <summary>
		/// Deconstructor for the compound axis.
		/// </summary>
		~AxisCompound();

		float GetAmount() const override;
	};
}
