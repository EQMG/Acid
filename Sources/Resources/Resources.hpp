#pragma once

#include "Engine/Engine.hpp"
#include "Helpers/ThreadPool.hpp"
#include "Files/Node.hpp"
#include "Resource.hpp"

namespace acid {
/**
 * @brief Module used for managing resources.
 */
class ACID_EXPORT Resources : public Module::Registrar<Resources, Module::Stage::Post> {
public:
	Resources();

	void Update() override;

	std::shared_ptr<Resource> Find(const Node &node) const;
	void Add(const Node &node, const std::shared_ptr<Resource> &resource);
	void Remove(const std::shared_ptr<Resource> &resource);

	/**
	 * Gets the resource loader thread pool.
	 * @return The resource loader thread pool.
	 */
	ThreadPool &GetThreadPool() { return m_threadPool; }

private:
	std::map<Node, std::shared_ptr<Resource>> m_resources;
	ElapsedTime m_elapsedPurge;

	ThreadPool m_threadPool;
};
}
