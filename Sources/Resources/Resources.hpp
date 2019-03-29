#pragma once

#include "Engine/Engine.hpp"
#include "Helpers/ThreadPool.hpp"
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

	/// <summary>
	/// Gets the resoure loader thread pool.
	/// </summary>
	/// <returns> The resoure loader thread pool. </returns>
	ThreadPool &GetThreadPool() { return m_threadPool; }

private:
	std::map<std::unique_ptr<Metadata>, std::shared_ptr<Resource>> m_resources;
	Timer m_timerPurge;

	ThreadPool m_threadPool;
};
}
