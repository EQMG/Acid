#pragma once

#include <iostream>
#include <map>
#include <string>

#include "ifactoryobject.h"
#include "ifactorybuilder.h"

namespace flounder {
	/// <summary>
	/// A class that represents a factory and its basic functionality.
	/// </summary>
	class ifactory 
	{
	private:
		std::string m_factoryName;
	public:
		/// <summary>
		/// Creates a new factory.
		/// </summary>
		/// <param name="factoryName"> The name of the factory. </param>
		ifactory(const std::string &factoryName) 
		{
			m_factoryName = factoryName;
		}

		/// <summary>
		/// Deconstructor for the factory.
		/// </summary>
		~ifactory()
		{
		}

		/// <summary>
		/// Creates a new empty object of the factories type.
		/// </summary>
		/// <returns> The new object. </returns>
		virtual ifactoryobject *newObject() = 0;

		/// <summary>
		/// Used to load resource data into a factory object, using building parameters.
		/// </summary>
		/// <param name="object"> The object to load data into. </param>
		/// <param name="builder"> The builder to use parameters from. </param>
		/// <param name="name"> The name of the object being loaded. </param>
		virtual void loadData(ifactoryobject *object, ifactorybuilder *builder, const std::string &name) = 0;

		/// <summary>
		/// Used to take resource data and turn it into the object. Can be used to load into OpenGL.
		/// </summary>
		/// <param name="object"> The object to create for. </param>
		/// <param name="builder"> The builder to use parameters from. </param>
		virtual void create(ifactoryobject *object, ifactorybuilder *builder) = 0;

		/// <summary>
		/// Gets a static list of all loaded factory objects. This is used to hold a list of already loaded objects to reduce load requests.
		/// </summary>
		/// <returns> The map of loaded factory objects, mapped by name. </returns>
		virtual std::map<std::string, ifactoryobject*> *getLoaded() = 0;

		/// <summary>
		/// The name of the factory.
		/// </summary>
		/// <returns> The factories name. </returns>
		inline std::string getFactoryName() { return m_factoryName; }
	};
}