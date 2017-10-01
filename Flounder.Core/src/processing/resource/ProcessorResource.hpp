#pragma once

#include "RequestResource.hpp"
#include "../IProcessor.hpp"
#include "../Queue.hpp"

namespace Flounder
{
	/// <summary>
	/// A extension that is responsible for processing resource requests.
	/// </summary>
	class ProcessorResource :
		public IProcessor
	{
	private:
		static const double MAX_TIME_MILLIS;

		Queue<RequestResource*> *m_queue;
	public:
		/// <summary>
		/// Creates a new resource processor.
		/// </summary>
		ProcessorResource();

		/// <summary>
		/// Deconstructor for the resource processor.
		/// </summary>
		~ProcessorResource();

		void Update() override;

		void AddRequestToQueue(IRequest *request) override;

		/// <summary>
		/// Completes all requests left in queue.
		/// </summary>
		void CompleteAllRequests() const;
	};
}
