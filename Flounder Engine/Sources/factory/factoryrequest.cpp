#include "factoryrequest.h"

namespace flounder {
	factoryrequest::factoryrequest(const std::string & name, ifactory * factory, ifactorybuilder * builder, ifactoryobject * object)
	{
		m_name = name;
		m_factory = factory;
		m_object = object;
		m_builder = builder;
	}

	factoryrequest::~factoryrequest()
	{
	}

	void factoryrequest::executeRequestResource()
	{
		// Loads resource data into the object.
		m_factory->loadData(m_object, m_builder, m_name);
	}

	void factoryrequest::executeRequestGL()
	{
		timer *wait = NULL;

		while (m_object == NULL || !m_object->isDataLoaded())
		{
			if (wait == NULL)
			{
			//	logger::get()->warning("Factory request for " + m_name + " is waiting for data!");
				wait = new timer(5.0); // Waits 5 seconds for data, if it stays unloaded then this object is not loaded.
			}

			if (wait->isPassedTime())
			{
				if (m_object != NULL)
				{
					m_object->setDataLoaded(false);
					m_object->setFullyLoaded(false);
				}

				wait->resetStartTime();
			//	logger::get()->error("Factory request for " + m_name + " failed! The object will not be loaded!");
			}

			// Wait for resources to load into data...
		}

		if (wait != NULL)
		{
			delete wait;
		}

		// Creates the object and sets as loaded.
		m_factory->create(m_object, m_builder);
	}
}
