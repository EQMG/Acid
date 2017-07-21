#pragma once

#include "../framework/framework.h"

#include "resource\processorresource.h"
#include "opengl\processoropengl.h"
#include "iprocessor.h"
#include "irequest.h"

namespace flounder
{
	class processing :
		public imodule
	{
	private:
		std::vector<iprocessor*> *m_processors;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static processing *get()
		{
			return static_cast<processing*>(framework::get()->getInstance("processing"));
		}

		processing();

		~processing();

		void init() override;

		void update() override;

		void addProcessor(iprocessor *processor);

		/// <summary>
		/// Sends a new resource request to be added to a que.
		/// </summary>
		/// <param name="request"> The resource request to add. </param>
		void sendRequest(irequest *request);
	};
}
