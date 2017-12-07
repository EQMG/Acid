#pragma once

#include "../IRequest.hpp"

namespace Flounder
{
	/// <summary>
	/// Interface for executable resource requests.
	/// </summary>
	class RequestResource :
		public IRequest
	{
	public:
		/// <summary>
		/// Executed when the request is being processed.
		/// </summary>
		virtual void ExecuteRequestResource() = 0;

		int GetRequestId() override { return 1; };
	};
}
