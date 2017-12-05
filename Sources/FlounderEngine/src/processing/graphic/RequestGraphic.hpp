#pragma once

#include "../IRequest.hpp"

namespace Flounder
{
	/// <summary>
	/// Interface for executable OpenGL requests.
	/// </summary>
	class RequestGraphic :
		public IRequest
	{
	public:
		/// <summary>
		/// Executed when the request is being processed.
		/// </summary>
		virtual void ExecuteRequestGraphic() = 0;

		int GetRequestId() override { return 2; };
	};
}
