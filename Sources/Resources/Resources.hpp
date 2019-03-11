#pragma once

#include "Engine/Engine.hpp"
#include "Maths/Timer.hpp"
#include "Serialized/Metadata.hpp"
#include "Resource.hpp"

namespace acid
{
	/// <summary>
	/// A module used for managing resources.
	/// </summary>
	class ACID_EXPORT Resources :
		public Module
	{
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Resources *Get() { return Engine::Get()->GetModuleManager().Get<Resources>(); }

		Resources();

		void Update() override;

		std::shared_ptr<Resource> Find(const Metadata &metadata) const;

		void Add(const Metadata &metadata, const std::shared_ptr<Resource> &resource);

		void Remove(const std::shared_ptr<Resource> &resource);
	private:
		std::map<std::unique_ptr<Metadata>, std::shared_ptr<Resource>> m_resources;
		Timer m_timerPurge;
	};
}
