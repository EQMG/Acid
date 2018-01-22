#pragma once

#include "RequestGraphic.hpp"
#include "../IProcessor.hpp"
#include "../Queue.hpp"

namespace Flounder
{
	/// <summary>
	/// A extension that is responsible for processing OpenGL requests.
	/// </summary>
	class ProcessorGraphic :
		public IProcessor
	{
	private:
		static const double MAX_TIME_MILLIS;

		Queue<RequestGraphic *> *m_queue;
	public:
		/// <summary>
		/// Creates a new opengl processor.
		/// </summary>
		ProcessorGraphic();

		/// <summary>
		/// Deconstructor for the opengl processor.
		/// </summary>
		~ProcessorGraphic();

		void Update() override;

		void AddRequestToQueue(IRequest *request) override;

		/// <summary>
		/// Completes all requests left in queue.
		/// </summary>
		void CompleteAllRequests() const;
	};
}
