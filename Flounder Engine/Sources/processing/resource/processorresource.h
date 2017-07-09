#pragma once

#include <iostream>
#include <thread>
#include "../iprocessor.h"
#include "../queue.h"
#include "requestresource.h"
#include "../../framework/framework.h"

namespace flounder {
	/// <summary>
	/// A extension that is responsible for processing resource requests.
	/// </summary>
	class processorresource : public iprocessor
	{
	private:
		static const double MAX_TIME_MILLIS;

		queue<requestresource*> *m_queue;
	public:
		processorresource();

		~processorresource();

		void update() override;

		void addRequestToQueue(irequest *request) override;

		/// <summary>
		/// Completes all requests left in queue.
		/// </summary>
		void completeAllRequests();

		int getRequestId() override;
	};
}
