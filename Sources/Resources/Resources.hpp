#pragma once

#include "Engine/Engine.hpp"
#include "Helpers/ThreadPool.hpp"
#include "Serialized/Metadata.hpp"
#include "Resource.hpp"

namespace acid
{
/**
 * @brief Module used for managing resources.
 */
class Resources :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Resources *Get() { return Engine::Get()->GetModule<Resources>(); }

	Resources();

	void Update() override;

	std::shared_ptr<Resource> Find(const Metadata &metadata) const;

	void Add(const Metadata &metadata, const std::shared_ptr<Resource> &resource);

	void Remove(const std::shared_ptr<Resource> &resource);

	/**
	 * Gets the resource loader thread pool.
	 * @return The resource loader thread pool.
	 */
	ThreadPool &GetThreadPool() { return m_threadPool; }

private:
	std::map<std::unique_ptr<Metadata>, std::shared_ptr<Resource>> m_resources;
	ElapsedTime m_elapsedPurge;

	ThreadPool m_threadPool;
};
}
