#pragma once

#include "../framework/framework.h"
#include "iprocessor.h"
#include "irequest.h"

#include "resource\processorresource.h"
#include "opengl\processoropengl.h"

namespace flounder {
	class processing : public module
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

		void update();

		/// <summary>
		/// Sends a new resource request to be added to a que.
		/// </summary>
		/// <param name="request"> The resource request to add. </param>
		void sendRequest(irequest *request);
	};
}
