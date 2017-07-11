#pragma once

#include <string>
#include <map>
#include "../logger/logger.h"
#include "../processing/processing.h"
#include "ifactory.h"
#include "ifactoryobject.h"
#include "factoryrequest.h"

namespace flounder {
	/// <summary>
	/// A builder used to set parameters for loading.
	/// </summary>
	class ifactorybuilder 
	{
	private:
		ifactory *m_factory;
	public:
		/// <summary>
		/// Creates a new builder.
		/// </summary>
		/// <param name="factory"> The factory to be used with. </param>
		ifactorybuilder(ifactory *factory)
		{
			m_factory = factory;
		}

		~ifactorybuilder() 
		{
			// Does not delete the factory pointer as it is passed from the factory class.
		}

		/// <summary>
		/// Creates a new factory object, carries out the CPU loading, and then runs on the OpenGL thread.
		/// </summary>
		/// <returns> The factory object that has been created. </returns>
		virtual ifactoryobject *create() = 0;

		/// <summary>
		/// Only call from <seealso cref="#create()"/>! Creates the model object.
		/// </summary>
		/// <param name="name"> The name to be referenced by. This may need to be loaded though the builder.
		/// </param>
		/// <returns> The factory object that has been created. </returns>
		ifactoryobject *builderCreate(std::string name)
		{
			ifactoryobject *object = m_factory->getLoaded()->find(name)->second;

			if (object == NULL)
			{
				logger::get()->log(name + " is being loaded into the " + m_factory->getFactoryName() + " factory right now!");

				m_factory->getLoaded()->erase(name);
				object = m_factory->newObject();
				processing::get()->sendRequest(new factoryrequest(name, m_factory, this, object));
				m_factory->getLoaded()->insert(name, object);
			}

			return object;
		}
	};
}