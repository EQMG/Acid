#include "Processing.hpp"

#include "resource/ProcessorResource.hpp"
#include "graphic/ProcessorGraphic.hpp"

namespace Flounder
{
	Processing::Processing() :
		IModule(),
		m_processors(new std::vector<IProcessor*>())
	{
		// Manually adds the two base processors, these will be added into the modules loop, but are needed now.
		// If these are not added in the init loop, nothing will be able to be initially processed!
		m_processors->push_back(new ProcessorResource());
		m_processors->push_back(new ProcessorGraphic());
	}

	Processing::~Processing()
	{
		for (auto processor : *m_processors)
		{
			delete processor;
		}

		delete m_processors;
	}

	void Processing::Update()
	{
		for (auto processor : *m_processors)
		{
			processor->Update();
		}
	}

	void Processing::AddProcessor(IProcessor *processor) const
	{
		m_processors->push_back(processor);
	}

	void Processing::SendRequest(IRequest *request) const
	{
		for (auto processor : *m_processors)
		{
			processor->AddRequestToQueue(request);
		}
	}
}
