#pragma once

#include "IRequest.hpp"

namespace Flounder
{
	/// <summary>
	/// A extension used with the processing class to define a processor.
	/// </summary>
	class IProcessor
	{
	public:
		/// <summary>
		/// Creates a new processor.
		/// </summary>
		IProcessor()
		{
		}

		/// <summary>
		/// Deconstructor for the processor.
		/// </summary>
		virtual ~IProcessor()
		{
		}

		/// <summary>
		/// Run when updating the processor.
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// Used to add a request into the processor.
		/// </summary>
		/// <param name="request"> The request object to add to the que. </param>
		virtual void AddRequestToQueue(IRequest *request) = 0;
	};
}
