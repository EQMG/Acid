#pragma once

#include "../framework/framework.h"

#include "resource\processorresource.h"
#include "opengl\processoropengl.h"
#include "iprocessor.h"
#include "irequest.h"

namespace flounder {
	class processing : 
		public imodule
	{
	private:
		std::vector<iprocessor*> *m_processors;
	public:
		static processing *get()
		{
			return (processing*) framework::get()->getInstance("processing");
		}

		processing();

		~processing();

		void loadProcessor(iprocessor *processor);

		void init();

		void update();

		/// <summary>
		/// Sends a new resource request to be added to a que.
		/// </summary>
		/// <param name="request"> The resource request to add. </param>
		void sendRequest(irequest *request);
	};
}
