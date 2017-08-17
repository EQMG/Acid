#include "processing.h"

namespace flounder
{
	processing::processing() :
		imodule()
	{
		m_processors = new std::vector<iprocessor*>();

		// Manually adds the two base processors, these will be added into the modules loop, but are needed now.
		// If these are not added in the init loop, nothing will be able to be initially processed!
		m_processors->push_back(new processorresource());
		m_processors->push_back(new processorgraphic());
	}

	processing::~processing()
	{
		for (iprocessor *processor : *m_processors)
		{
			delete processor;
		}

		delete m_processors;
	}

	void processing::addProcessor(iprocessor *processor)
	{
		m_processors->push_back(processor);
	}

	void processing::update()
	{
		for (iprocessor *processor : *m_processors)
		{
			processor->update();
		}
	}

	void processing::sendRequest(irequest *request)
	{
		for (auto processor : *m_processors)
		{
			processor->addRequestToQueue(request);
		}
	}
}
