#pragma once

#include "../iprocessor.h"
#include "../queue.h"
#include "requestopengl.h"
#include "../../framework/framework.h"

namespace flounder {
	/// <summary>
	/// A extension that is responsible for processing OpenGL requests.
	/// </summary>
	class processoropengl : public iprocessor
	{
	private:
		static const double MAX_TIME_MILLIS;

		queue<requestopengl*> *m_queue;
	public:
		processoropengl();

		~processoropengl();

		void update() override;

		void addRequestToQueue(irequest *request) override;

		/// <summary>
		/// Completes all requests left in queue.
		/// </summary>
		void completeAllRequests();

		int getRequestId() override;
	};
}
