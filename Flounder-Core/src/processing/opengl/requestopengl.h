#pragma once

#include "../irequest.h"

namespace flounder 
{
	/// <summary>
	/// Interface for executable OpenGL requests.
	/// </summary>
	class requestopengl : 
		public irequest 
	{
	public:
		/// <summary>
		/// Executed when the request is being processed.
		/// </summary>
		virtual void executeRequestGL() = 0;

		inline int getRequestId() override { return 2; };
	};
}