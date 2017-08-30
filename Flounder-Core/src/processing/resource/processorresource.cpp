#include "processorresource.hpp"

namespace flounder
{
	const double processorresource::MAX_TIME_MILLIS = 8.0f;

	processorresource::processorresource() :
		m_queue(new queue<requestresource*>())
	{
	}

	processorresource::~processorresource()
	{
		delete m_queue;
	}

	void processorresource::update()
	{
		if (!m_queue->hasRequests())
		{
			return;
		}

		double remainingTime = MAX_TIME_MILLIS;
		double start = framework::get()->getTimeMs();

		while (m_queue->hasRequests())
		{
			requestresource *request = m_queue->acceptNextRequest();
			request->executeRequestResource();
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

	void processorresource::addRequestToQueue(irequest *request)
	{
		if (dynamic_cast<requestresource*>(request) == nullptr)
		{
			return;
		}

		m_queue->addRequest((requestresource*) request);
	}

	void processorresource::completeAllRequests()
	{
		while (m_queue->hasRequests())
		{
			m_queue->acceptNextRequest()->executeRequestResource();
		}
	}
}
