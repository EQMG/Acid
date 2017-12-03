#include "ProcessorResource.hpp"

#include "../../engine/Engine.hpp"

namespace Flounder
{
	const double ProcessorResource::MAX_TIME_MILLIS = 8.0f;

	ProcessorResource::ProcessorResource() :
		m_queue(new Queue<RequestResource*>())
	{
	}

	ProcessorResource::~ProcessorResource()
	{
		delete m_queue;
	}

	void ProcessorResource::Update()
	{
		if (!m_queue->HasRequests())
		{
			return;
		}

		double remainingTime = MAX_TIME_MILLIS;
		double start = Engine::Get()->GetTimeMs();

		while (m_queue->HasRequests())
		{
			RequestResource *request = m_queue->AcceptNextRequest();
			request->ExecuteRequestResource();
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

	void ProcessorResource::AddRequestToQueue(IRequest *request)
	{
		if (dynamic_cast<RequestResource*>(request) == nullptr)
		{
			return;
		}

		m_queue->AddRequest(static_cast<RequestResource*>(request));
	}

	void ProcessorResource::CompleteAllRequests() const
	{
		while (m_queue->HasRequests())
		{
			m_queue->AcceptNextRequest()->ExecuteRequestResource();
		}
	}
}
