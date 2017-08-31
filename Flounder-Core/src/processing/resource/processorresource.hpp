#pragma once

#include "../../engine/Engine.hpp"

#include "requestresource.hpp"
#include "../iprocessor.hpp"
#include "../queue.hpp"

namespace Flounder
{
	/// <summary>
	/// A extension that is responsible for processing resource requests.
	/// </summary>
	class processorresource :
		public iprocessor
	{
	private:
		static const double MAX_TIME_MILLIS;

		queue<requestresource*> *m_queue;
	public:
		/// <summary>
		/// Creates a new resource processor.
		/// </summary>
		processorresource();

		/// <summary>
		/// Deconstructor for the resource processor.
		/// </summary>
		~processorresource();

		void update() override;

		void addRequestToQueue(irequest *request) override;

		/// <summary>
		/// Completes all requests left in queue.
		/// </summary>
		void completeAllRequests();
	};
}
