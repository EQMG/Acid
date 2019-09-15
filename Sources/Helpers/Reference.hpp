#pragma once

#include "StdAfx.hpp"

namespace acid {
template<typename T>
class Reference {
public:
	Reference() noexcept = default;
	
	Reference(T &reference) noexcept :
		m_reference(std::addressof(reference)) {
	}
	
	~Reference() = default;

	/**
	 * Access the stored reference.
	 * @return The reference.
	 */
	T &get() const noexcept { return *m_reference; }

	/**
	 * Access the stored reference.
	 * @return The reference.
	 */
	constexpr operator T &() const noexcept { return *m_reference; }

	/**
	 * Gets the address of the stored reference.
	 * @return The reference.
	 */
	T *operator&() const noexcept { return m_reference; }

	/**
	 * Call the function of the stored reference.
	 * @return The reference.
	 */
	T *operator->() const noexcept { return m_reference; }

	/**
	 * Calls the stored function.
	 * @tparam Args The arg types to pass.
	 * @param args The arguments.
	 * @return The invoked result.
	 */
	template<typename... Args>
	std::invoke_result_t<T &, Args...> operator()(Args &&...args) const {
		return std::invoke(get(), std::forward<Args>(args)...);
	}

	bool operator==(const Reference &other) const noexcept {
		return m_reference == other.m_reference;
	}

	bool operator!=(const Reference &other) const noexcept {
		return !(*this == other);
	}

private:
	// Address of the referenced object.
	T *m_reference = nullptr;
};
}
