#pragma once

#include <stdarg.h>

#include "../maths/maths.h"

#include "iaxis.h"

namespace flounder
{
	/// <summary>
	/// Axis composed of multiple other axes.
	/// </summary>
	class axiscompound :
		public iaxis
	{
	private:
		int m_count;
		iaxis **m_axes;
	public:
		/// <summary>
		/// Creates a new compound axis.
		/// </summary>
		/// <param name="n_args"> The number of axes being added. </param>
		/// <param name="..."> The axes on the being added. </param>
		axiscompound(const int n_args, ...);

		/// <summary>
		/// Deconstructor for the compound axis.
		/// </summary>
		~axiscompound();

		float getAmount() const override;
	};
}
