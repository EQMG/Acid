#pragma once

#include <vector>
#include "../Engine/Engine.hpp"
#include "IProcessor.hpp"
#include "IRequest.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for processing types of requests.
	/// </summary>
	class Processing :
		public IModule
	{
	private:
		std::vector<IProcessor *> *m_processors;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Processing *Get()
		{
			return dynamic_cast<Processing *>(Engine::Get()->GetModule("processing"));
		}

		/// <summary>
		/// Creates a new processing module.
		/// </summary>
		Processing();

		/// <summary>
		/// Deconstructor for the processing module.
		/// </summary>
		~Processing();

		void Update() override;

		/// <summary>
		/// Adds a processor type to the list of processors.
		/// </summary>
		/// <param name="processor"> The processor to add. </param>
		void AddProcessor(IProcessor *processor) const;

		/// <summary>
		/// Sends a new resource request to be added to a que.
		/// </summary>
		/// <param name="request"> The resource request to add. </param>
		void SendRequest(IRequest *request) const;
	};
}
