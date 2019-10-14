#pragma once

#include "SystemHolder.hpp"

namespace acid {
template<typename T>
bool SystemHolder::HasSystem() const {
	const auto it = m_systems.find(GetSystemTypeId<T>());

	return it != m_systems.end() && it->second;
}

template<typename T>
T *SystemHolder::GetSystem() const {
	auto it = m_systems.find(GetSystemTypeId<T>());

	if (it == m_systems.end() || !it->second) {
		//throw std::runtime_error("Scene does not have requested System");
		return nullptr;
	}

	return static_cast<T *>(it->second.get());
}

template<typename T>
void SystemHolder::AddSystem(std::size_t priority, std::unique_ptr<T> &&system) {
	// Remove previous System, if it exists.
	RemoveSystem<T>();

	const auto typeId = GetSystemTypeId<T>();

	// Insert the priority value
	m_priorities.insert({priority, typeId});

	// Then, add the System
	m_systems[typeId] = std::move(system);
}

template<typename T>
void SystemHolder::RemoveSystem() {
	const auto typeId = GetSystemTypeId<T>();

	auto system = m_systems.find(typeId);

	if (system != m_systems.end() && system->second) {
		system->second->DetachAll();
	}

	// Remove the priority value for this System.
	RemoveSystemPriority(typeId);

	// Then, remove the System.
	m_systems.erase(typeId);
}

template<typename Func>
void SystemHolder::ForEach(Func &&func) {
	for (const auto &typeId : m_priorities) {
		if (auto &system = m_systems[typeId.second]) {
			try {
				func(*system, typeId.second);
			} catch (const std::exception &e) {
				std::cerr << e.what() << '\n';
			}
		}
	}
}
}
