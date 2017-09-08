#pragma once

#include <stdarg.h>

#include "../maths/maths.hpp"

#include "iaxis.hpp"

namespace Flounder
{
	/// <summary>
	/// Axis composed of multiple other axes.
	/// </summary>
	class AxisCompound :
		public IAxis
	{
	private:
		int m_count;
		IAxis **m_axes;
	public:
		/// <summary>
		/// Creates a new compound axis.
		/// </summary>
		/// <param name="n_args"> The number of axes being added. </param>
		/// <param name="..."> The axes on the being added. </param>
		AxisCompound(const int n_args, ...);

		/// <summary>
		/// Deconstructor for the compound axis.
		/// </summary>
		~AxisCompound();

		float GetAmount() const override;
	};
}
