#pragma once

#include <mutex>
#include "ConstExpr.hpp"
#include "NonCopyable.hpp"

namespace acid {
template<typename>
class Delegate;

class ACID_EXPORT Observer {
public:
	Observer() :
		m_valid(std::make_shared<bool>(true)) {
	}

	virtual ~Observer() = default;

	std::shared_ptr<bool> m_valid;
};

template<typename TReturnType, typename ...TArgs>
class Invoker {
public:
	using ReturnType = std::vector<TReturnType>;

	static ReturnType Invoke(Delegate<TReturnType(TArgs ...)> &delegate, TArgs ... params) {
		std::lock_guard<std::mutex> lock(delegate.m_mutex);
		ReturnType returnValues;

		for (auto it = delegate.m_functions.begin(); it != delegate.m_functions.end();) {
			if (it->IsExpired()) {
				it = delegate.m_functions.erase(it);
				continue;
			}

			returnValues.emplace_back((*it->m_function)(params...));
			++it;
		}

		return returnValues;
	}
};

template<typename... TArgs>
class Invoker<void, TArgs...> {
public:
	using ReturnType = void;

	static void Invoke(Delegate<void(TArgs ...)> &delegate, TArgs ... params) {
		std::lock_guard<std::mutex> lock(delegate.m_mutex);

		if (delegate.m_functions.empty()) {
			return;
		}

		for (auto it = delegate.m_functions.begin(); it != delegate.m_functions.end();) {
			if (it->IsExpired()) {
				it = delegate.m_functions.erase(it);
				continue;
			}

			it->m_function(params...);
			++it;
		}
	}
};

template<typename TReturnType, typename ...TArgs>
class Delegate<TReturnType(TArgs ...)> {
public:
	using Invoker = acid::Invoker<TReturnType, TArgs...>;
	using FunctionType = std::function<TReturnType(TArgs ...)>;
	using ObserversType = std::vector<std::weak_ptr<bool>>;

	struct FunctionPair {
		FunctionType m_function;
		ObserversType m_observers;

		bool IsExpired() {
			for (const auto &observer : m_observers) {
				if (observer.expired()) {
					return true;
				}
			}

			return false;
		}
	};

	Delegate() = default;
	virtual ~Delegate() = default;

	template<typename ...KArgs>
	void Add(FunctionType &&function, KArgs ...args) {
		std::lock_guard<std::mutex> lock(m_mutex);
		ObserversType observers;

		if constexpr (sizeof...(args) != 0) {
			for (const auto &arg : {args...}) {
				observers.emplace_back(ConstExpr::AsPtr(arg)->m_valid);
			}
		}

		m_functions.emplace_back(FunctionPair{std::move(function), observers});
	}

	void Remove(const FunctionType &function) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_functions.erase(std::remove_if(m_functions.begin(), m_functions.end(), [function](FunctionPair &f) {
			return Hash(f.m_function) == Hash(function);
		}), m_functions.end());
	}

	void MoveFunctions(Delegate &from) {
		m_functions = std::move(from.m_functions);
	}

	void Clear() {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_functions.clear();
	}

	typename Invoker::ReturnType Invoke(TArgs ... args) {
		return Invoker::Invoke(*this, args...);
	}

	Delegate &operator+=(FunctionType &&function) {
		return Add(std::move(function));
	}

	Delegate &operator-=(const FunctionType function) {
		return Remove(function);
	}

	typename Invoker::ReturnType operator()(TArgs ... args) {
		return Invoker::Invoke(*this, args...);
	}

private:
	friend Invoker;

	static constexpr size_t Hash(const FunctionType &function) {
		return function.target_type().hash_code();
	}

	std::mutex m_mutex;
	std::vector<FunctionPair> m_functions;
};

template<typename T>
class DelegateValue :
	public Delegate<void(T)>,
	public virtual NonCopyable {
public:
	template<typename ...Args>
	DelegateValue(Args ...args) :
		m_value(std::forward<Args>(args)...) {
	}

	virtual ~DelegateValue() = default;

	DelegateValue &operator=(T value) {
		m_value = value;
		Invoke(m_value);
		return *this;
	}

	/**
	 * Access the stored value.
	 * @return The value.
	 */
	constexpr operator const T &() const noexcept { return m_value; }

	const T &get() const { return m_value; }
	const T &operator*() const { return m_value; }
	const T *operator->() const { return &m_value; }

protected:
	T m_value;
};
}
