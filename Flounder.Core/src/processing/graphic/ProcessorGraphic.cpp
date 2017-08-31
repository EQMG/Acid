#include "processorgraphic.hpp"

namespace Flounder
{
	const double processorgraphic::MAX_TIME_MILLIS = 8.0f;

	processorgraphic::processorgraphic() :
		m_queue(new queue<requestgraphic*>())
	{
	}

	processorgraphic::~processorgraphic()
	{
		delete m_queue;
	}

	void processorgraphic::update()
	{
		if (!m_queue->hasRequests())
		{
			return;
		}

		double remainingTime = MAX_TIME_MILLIS;
		double start = Engine::Get()->GetTimeMs();

		while (m_queue->hasRequests())
		{
			requestgraphic *request = m_queue->acceptNextRequest();
			request->executeRequestGraphic();
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

	void processorgraphic::addRequestToQueue(irequest *request)
	{
		if (dynamic_cast<requestgraphic*>(request) == nullptr)
		{
			return;
		}

		m_queue->addRequest((requestgraphic*) request);
	}

	void processorgraphic::completeAllRequests()
	{
		while (m_queue->hasRequests())
		{
			m_queue->acceptNextRequest()->executeRequestGraphic();
		}
	}
}
