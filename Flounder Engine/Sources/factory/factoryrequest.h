#pragma once

#include <string>

#include "../processing/resource/requestresource.h"
#include "../processing/opengl/requestopengl.h"

#include "../maths/timer.h"

#include "ifactory.h"
#include "ifactorybuilder.h"

namespace flounder {
	/// <summary>
	/// A class that can process a request to load a factory object.
	/// </summary>
	class factoryrequest : public requestresource, requestopengl
	{
	private:
		std::string m_name;
		ifactory *m_factory;
		ifactorybuilder *m_builder;
		ifactoryobject *m_object;
	public:
		/// <summary>
		/// Creates a new factory load request.
		/// </summary>
		/// <param name="name"> The name of the object being loaded. </param>
		/// <param name="factory"> The factory to use when executing requests. </param>
		/// <param name="builder"> The builder to load from. </param>
		/// <param name="object"> The object to load into. </param>
		factoryrequest(const std::string &name, ifactory *factory, ifactorybuilder *builder, ifactoryobject *object);

		~factoryrequest();

		void executeRequestResource() override;

		void executeRequestGL() override;
	};
}
