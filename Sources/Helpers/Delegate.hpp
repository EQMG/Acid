#pragma once

#include <algorithm>
#include <functional>
#include <memory>
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
				observers.emplace_back(to_address(arg)->m_valid);
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

	template<typename ...KArgs>
	void RemoveObservers(KArgs ...args) {
		ObserversType removes;

		if constexpr (sizeof...(args) != 0) {
			for (const auto &arg : {args...}) {
				removes.emplace_back(to_address(arg)->m_valid);
			}
		}
		
		for (auto it = m_functions.begin(); it != m_functions.end();) {
			for (auto it1 = it->m_observers.begin(); it1 != it->m_observers.end();) {
				bool erase = false;
				auto opt = it1->lock();
				for (const auto &remove : removes) {
					auto ept = remove.lock();
					if (opt.get() == ept.get())
						erase = true;
				}
				if (erase)
					it1 = it->m_observers.erase(it1);
				else
					++it1;
			}

			if (it->m_observers.empty())
				it = m_functions.erase(it);
			else
				++it;
		}
	}

	void MoveFunctions(Delegate &from, const ObserversType &exclude = {}) {
		for (auto it = from.m_functions.begin(); it < from.m_functions.end();) {
			bool move = true;
			for (const auto &excluded : exclude) {
				auto ept = excluded.lock();
				for (const auto &observer : it->m_observers) {
					auto opt = observer.lock();
					if (opt.get() == ept.get())
						move = false;
				}
			}

			if (move) {
				std::move(from.m_functions.begin(), it, std::back_inserter(m_functions));
				it = from.m_functions.erase(from.m_functions.begin(), it);
			} else {
				++it;
			}
		}
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
class DelegateValue : public Delegate<void(T)>, NonCopyable {
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
	operator const T &() const noexcept { return m_value; }

	const T &get() const { return m_value; }
	const T &operator*() const { return m_value; }
	const T *operator->() const { return &m_value; }

protected:
	T m_value;
};
}
