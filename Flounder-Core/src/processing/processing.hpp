#pragma once

#include "../engine/Engine.hpp"

#include "resource/processorresource.hpp"
#include "graphic/processorgraphic.hpp"
#include "iprocessor.hpp"
#include "irequest.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for processing types of requests.
	/// </summary>
	class processing :
		public IModule
	{
	private:
		std::vector<iprocessor*> *m_processors;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline processing *get()
		{
			return static_cast<processing*>(Engine::Get()->GetModule("processing"));
		}

		/// <summary>
		/// Creates a new processing module.
		/// </summary>
		processing();

		/// <summary>
		/// Deconstructor for the processing module.
		/// </summary>
		~processing();

		void Update() override;

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
