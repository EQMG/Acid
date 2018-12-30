#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include "Engine/Engine.hpp"
#include "Maths/Timer.hpp"
#include "IResource.hpp"

namespace acid
{
	/// <summary>
	/// A module used for managing resources.
	/// </summary>
	class ACID_EXPORT Resources :
		public Module
	{
	private:
		std::mutex m_mutex;
		std::vector<std::shared_ptr<IResource>> m_resources;
		Timer m_timerPurge;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Resources *Get() { return Engine::Get()->GetModuleManager().Get<Resources>(); }

		Resources();

		void Update() override;

		std::shared_ptr<IResource> Find(const std::string &filename) const;

		void Add(const std::shared_ptr<IResource> &resource);

		bool Remove(const std::shared_ptr<IResource> &resource);

		bool Remove(const std::string &filename);
	};
}
