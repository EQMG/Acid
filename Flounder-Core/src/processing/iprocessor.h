#pragma once

#include "irequest.h"

namespace flounder
{
	/// <summary>
	/// A extension used with the processing class to define a processor.
	/// </summary>
	class iprocessor
	{
	public:
		/// <summary>
		/// Creates a new processor.
		/// </summary>
		iprocessor()
		{
		}

		/// <summary>
		/// Deconstructor for the processor.
		/// </summary>
		virtual ~iprocessor()
		{
		}

		/// <summary>
		/// Run when updating the processor.
		/// </summary>
		virtual void update() = 0;

		/// <summary>
		/// Used to add a request into the processor.
		/// </summary>
		/// <param name="request"> The request object to add to the que. </param>
		virtual void addRequestToQueue(irequest *request) = 0;
	};
}
