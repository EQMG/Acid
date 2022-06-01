#pragma once

#include "Files/Node.hpp"

namespace acid {
/**
 * @brief Class that allows {@link Node::Get()} to handle returning XML container types (std::vector, std::array)
 * where the XML array may contain 1 or n objects.
 */
template<typename T, typename = std::enable_if_t<is_container_v<std::remove_reference_t<T>>>>
class XmlContainer {
public:
	constexpr XmlContainer(const T &value = {}) : value(value) {}
	template<typename T1 = T, typename = std::enable_if_t<!std::is_reference_v<T1>>>
	constexpr XmlContainer(T1 &&value) : value(std::move(value)) {}
	
	friend const Node &operator>>(const Node &node, XmlContainer<T> &object) {
		if (node.GetType() == NodeType::Array)
			return node >> object.value;
		return node >> object.value.emplace_back();
	}

	friend Node &operator<<(Node &node, const XmlContainer<T> &object) {
		return node << object.value;
	}

	T operator*() noexcept { return std::move(value); }
	operator T () noexcept { return std::move(value); }

	template<typename K = std::remove_reference_t<T>>
	constexpr XmlContainer &operator=(const K &value) noexcept {
		this->value = value;
		return *this;
	}

	template<typename K = std::remove_reference_t<T>>
	constexpr XmlContainer &operator=(K &&value) noexcept {
		this->value = std::move(value);
		return *this;
	}

protected:
	T value;
};

template<typename T>
class XmlContainerRef : public XmlContainer<T &> {
public:
	constexpr XmlContainerRef(T &value) : XmlContainer<T &>(value) {}
	
	operator const T &() const { return this->value; }
	const T &operator*() const { return this->value; }

	template<typename K>
	constexpr XmlContainerRef &operator=(const K &value) noexcept {
		this->value = value;
		return *this;
	}

	template<typename K>
	constexpr XmlContainerRef &operator=(K &&value) noexcept {
		this->value = std::move(value);
		return *this;
	}
};
}
