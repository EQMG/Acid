#include "ProcessorGraphic.hpp"

namespace Flounder
{
	const double ProcessorGraphic::MAX_TIME_MILLIS = 8.0f;

	ProcessorGraphic::ProcessorGraphic() :
		m_queue(new Queue<RequestGraphic*>())
	{
	}

	ProcessorGraphic::~ProcessorGraphic()
	{
		delete m_queue;
	}

	void ProcessorGraphic::Update()
	{
		if (!m_queue->HasRequests())
		{
			return;
		}

		double remainingTime = MAX_TIME_MILLIS;
		double start = Engine::Get()->GetTimeMs();

		while (m_queue->HasRequests())
		{
			RequestGraphic *request = m_queue->AcceptNextRequest();
			request->ExecuteRequestGraphic();
			double end = Engine::Get()->GetTimeMs();
			double timeTaken = end - start;
			remainingTime -= timeTaken;
			start = end;
			delete request;

			if (remainingTime < 0.0)
			{
				return;
			}
		}
	}

	void ProcessorGraphic::AddRequestToQueue(IRequest *request)
	{
		if (dynamic_cast<RequestGraphic*>(request) == nullptr)
		{
			return;
		}

		m_queue->AddRequest(static_cast<RequestGraphic*>(request));
	}

	void ProcessorGraphic::CompleteAllRequests() const
	{
		while (m_queue->HasRequests())
		{
			m_queue->AcceptNextRequest()->ExecuteRequestGraphic();
		}
	}
}
