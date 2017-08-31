#pragma once

#include "../irequest.hpp"

namespace Flounder
{
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

		int getRequestId() override { return 1; };
	};
}
