#pragma once

#include "../framework/framework.h"

#include "resource/processorresource.h"
#include "graphic/processorgraphic.h"
#include "iprocessor.h"
#include "irequest.h"

namespace flounder
{
	/// <summary>
	/// A module used for processing types of requests.
	/// </summary>
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

		/// <summary>
		/// Creates a new processing module.
		/// </summary>
		processing();

		/// <summary>
		/// Deconstructor for the processing module.
		/// </summary>
		~processing();

		void update() override;

		/// <summary>
		/// Adds a processor type to the list of processors.
		/// </summary>
		/// <param name="processor"> The processor to add. </param>
		void addProcessor(iprocessor *processor);

		/// <summary>
		/// Sends a new resource request to be added to a que.
		/// </summary>
		/// <param name="request"> The resource request to add. </param>
		void sendRequest(irequest *request);
	};
}
