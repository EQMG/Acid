#pragma once

#include "../../framework/framework.h"

#include "requestopengl.h"
#include "../iprocessor.h"
#include "../queue.h"

namespace flounder
{
	/// <summary>
	/// A extension that is responsible for processing OpenGL requests.
	/// </summary>
	class processoropengl : public iprocessor
	{
	private:
		static const double MAX_TIME_MILLIS;

		queue<requestopengl*> *m_queue;
	public:
		/// <summary>
		/// Creates a new opengl processor.
		/// </summary>
		processoropengl();

		/// <summary>
		/// Deconstructor for the opengl processor.
		/// </summary>
		~processoropengl();

		void update() override;

		void addRequestToQueue(irequest *request) override;

		/// <summary>
		/// Completes all requests left in queue.
		/// </summary>
		void completeAllRequests();
	};
}
