#pragma once

#include "Engine/Log.hpp"
#include "Files/Node.hpp"
#include "Helpers/Delegate.hpp"

namespace acid {
class Entity;

template<typename Base>
class ComponentFactory {
public:
	using TCreateReturn = std::unique_ptr<Base>;

	using TCreateMethod = std::function<TCreateReturn()>;
	using TRegistryMap = std::unordered_map<std::string, TCreateMethod>;

	using TCompareMethod = std::function<bool(const Base *)>;
	using TCompareVector = std::vector<std::pair<TCompareMethod, std::string>>;

	using TEncodeMethod = std::function<void(Node &, const Base *)>;
	using TDecodeMethod = std::function<void(const Node &, Base *)>;
	using TEncodeDecodeMap = std::unordered_map<std::string, std::pair<TEncodeMethod, TDecodeMethod>>;

	/**
	 * Creates a new component.
	 * @param name The component name to create.
	 * @return The new component.
	 */
	static TCreateReturn Create(const std::string &name) {
		auto it = Registry().find(name);
		if (it == Registry().end()) {
			Log::Error("Could not find registered component: ", std::quoted(name), '\n');
			return nullptr;
		}
		
		return it->second();
	}

	static std::string FindName(const Base *component) {
		for (const auto &[f, name] : RegistryCompare())
			if (f(component)) return name;
		return "";
	}

	static void Encode(const std::string &name, Node &node, const Base *component) {
		auto it = RegistryEncodeDecode().find(name);
		if (it == RegistryEncodeDecode().end()) {
			Log::Error("Could not find registered component: ", std::quoted(name), '\n');
			return;
		}
		
		it->second.first(node, component);
	}

	static void Decode(const std::string &name, const Node &node, Base *component) {
		auto it = RegistryEncodeDecode().find(name);
		if (it == RegistryEncodeDecode().end()) {
			Log::Error("Could not find registered component: ", std::quoted(name), '\n');
			return;
		}
		
		it->second.second(node, component);
	}

	static TRegistryMap &Registry() {
		static TRegistryMap impl;
		return impl;
	}

	static TCompareVector &RegistryCompare() {
		static TCompareVector impl;
		return impl;
	}

	static TEncodeDecodeMap &RegistryEncodeDecode() {
		static TEncodeDecodeMap impl;
		return impl;
	}

	template<typename T>
	class Registrar : public Base {
	public:
		Registrar(const std::string &name) {
			ComponentFactory::Registry()[name] = []() -> TCreateReturn {
				return std::make_unique<T>();
			};
			ComponentFactory::RegistryCompare().emplace_back([](const Base *component) {
				return dynamic_cast<const T *>(component);
			}, name);
			ComponentFactory::RegistryEncodeDecode()[name] = std::make_pair<TEncodeMethod, TDecodeMethod>(
				[](Node &node, const Base *component) {
					node << *dynamic_cast<const T *>(component);
				}, 
				[](const Node &node, Base *component) {
					node >> *dynamic_cast<T *>(component);
				}
			);
		}
		
		/*void Decode(Node &node, const T &component) override {
			node << component;
		}
		void Encode(const Node &node, T &component) override {
			node >> component;
		}*/
	};
};

/**
 * @brief Class that represents a functional component attached to entity.
 */
class ACID_EXPORT Component : public ComponentFactory<Component>, public virtual Observer {
	friend class Entity;
public:
	virtual ~Component() = default;

	/**
	 * Run when starting the component if {@link Component#m_started} is false.
	 */
	virtual void Start() {
	}

	/**
	 * Run when updating the entity this is attached to.
	 */
	virtual void Update() {
	}

	bool IsEnabled() const { return m_enabled; };
	void SetEnabled(bool enable) { m_enabled = enable; }

	bool IsRemoved() const { return m_removed; }
	void SetRemoved(bool removed) { m_removed = removed; }

	/**
	 * Gets the entity this component is attached to.
	 * @return The entity this component is attached to.
	 */
	Entity *GetEntity() const { return m_entity; }

	/**
	 * Sets the entity that this component is attached to.
	 * @param entity The new entity this is attached to.
	 */
	void SetEntity(Entity *entity) { m_entity = entity; }

private:
	bool m_started = false;
	bool m_enabled = true;
	bool m_removed = false;
	Entity *m_entity = nullptr;
};

}
