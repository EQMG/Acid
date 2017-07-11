#pragma once

namespace flounder {
	class irequest {
	public:
		/// <summary>
		/// Gets the request id.
		/// </summary>
		/// <returns> The request id used. </returns>
		virtual int getRequestId() = 0;
	};
}