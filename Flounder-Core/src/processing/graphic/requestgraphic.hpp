#pragma once

#include "../irequest.hpp"

namespace Flounder
{
	/// <summary>
	/// Interface for executable OpenGL requests.
	/// </summary>
	class requestgraphic :
		public irequest
	{
	public:
		/// <summary>
		/// Executed when the request is being processed.
		/// </summary>
		virtual void executeRequestGraphic() = 0;

		int getRequestId() override { return 2; };
	};
}
