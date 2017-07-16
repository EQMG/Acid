#pragma once

#include "../irequest.h"

namespace flounder {
	/// <summary>
	/// Interface for executable resource requests.
	/// </summary>
	class requestresource : 
		public irequest 
	{
	public:
		/// <summary>
		/// Executed when the request is being processed.
		/// </summary>
		virtual void executeRequestResource() = 0;

		inline int getRequestId() override { return 1; };
	};
}