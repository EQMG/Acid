#include "processorgraphic.h"

namespace flounder
{
	const double processorgraphic::MAX_TIME_MILLIS = 8.0f;

	processorgraphic::processorgraphic()
	{
		m_queue = new queue<requestgraphic*>();
	}

	processorgraphic::~processorgraphic()
	{
		delete m_queue;
	}

	void processorgraphic::update()
	{
		double remainingTime = MAX_TIME_MILLIS;
		double start = framework::get()->getTimeMs();

		while (m_queue->hasRequests())
		{
			requestgraphic *request = m_queue->acceptNextRequest();
			request->executeRequestGraphic();
			double end = framework::get()->getTimeMs();
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
