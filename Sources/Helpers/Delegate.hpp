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
		valid(std::make_shared<bool>(true)) {
	}

	virtual ~Observer() = default;

	std::shared_ptr<bool> valid;
};

template<typename TReturnType, typename ...TArgs>
class Invoker {
public:
	using ReturnType = std::vector<TReturnType>;

	static ReturnType Invoke(Delegate<TReturnType(TArgs ...)> &delegate, TArgs ... params) {
		std::lock_guard<std::mutex> lock(delegate.mutex);
		ReturnType returnValues;

		for (auto it = delegate.functions.begin(); it != delegate.functions.end();) {
			if (it->IsExpired()) {
				it = delegate.functions.erase(it);
				continue;
			}

			returnValues.emplace_back((*it->function)(params...));
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
		std::lock_guard<std::mutex> lock(delegate.mutex);

		if (delegate.functions.empty()) {
			return;
		}

		for (auto it = delegate.functions.begin(); it != delegate.functions.end();) {
			if (it->IsExpired()) {
				it = delegate.functions.erase(it);
				continue;
			}

			it->function(params...);
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

	class FunctionPair {
	public:
		bool IsExpired() {
			for (const auto &observer : observers) {
				if (observer.expired()) {
					return true;
				}
			}

			return false;
		}
		
		FunctionType function;
		ObserversType observers;
	};

	Delegate() = default;
	virtual ~Delegate() = default;

	template<typename ...KArgs>
	void Add(FunctionType &&function, KArgs ...args) {
		std::lock_guard<std::mutex> lock(mutex);
		ObserversType observers;

		if constexpr (sizeof...(args) != 0) {
			for (const auto &arg : {args...}) {
				observers.emplace_back(to_address(arg)->valid);
			}
		}

		functions.emplace_back(FunctionPair{std::move(function), observers});
	}

	void Remove(const FunctionType &function) {
		std::lock_guard<std::mutex> lock(mutex);
		functions.erase(std::remove_if(functions.begin(), functions.end(), [function](FunctionPair &f) {
			return Hash(f.function) == Hash(function);
		}), functions.end());
	}

	template<typename ...KArgs>
	void RemoveObservers(KArgs ...args) {
		ObserversType removes;

		if constexpr (sizeof...(args) != 0) {
			for (const auto &arg : {args...}) {
				removes.emplace_back(to_address(arg)->valid);
			}
		}

		for (auto it = functions.begin(); it != functions.end();) {
			for (auto it1 = it->observers.begin(); it1 != it->observers.end();) {
				bool erase = false;
				auto opt = it1->lock();
				for (const auto &remove : removes) {
					auto ept = remove.lock();
					if (opt.get() == ept.get())
						erase = true;
				}
				if (erase)
					it1 = it->observers.erase(it1);
				else
					++it1;
			}

			if (it->observers.empty())
				it = functions.erase(it);
			else
				++it;
		}
	}

	void MoveFunctions(Delegate &from, const ObserversType &exclude = {}) {
		for (auto it = from.functions.begin(); it < from.functions.end();) {
			bool move = true;
			for (const auto &excluded : exclude) {
				auto ept = excluded.lock();
				for (const auto &observer : it->observers) {
					auto opt = observer.lock();
					if (opt.get() == ept.get())
						move = false;
				}
			}

			if (move) {
				std::move(from.functions.begin(), it, std::back_inserter(functions));
				it = from.functions.erase(from.functions.begin(), it);
			} else {
				++it;
			}
		}
	}

	void Clear() {
		std::lock_guard<std::mutex> lock(mutex);
		functions.clear();
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

	std::mutex mutex;
	std::vector<FunctionPair> functions;
};

template<typename T>
class DelegateValue : public Delegate<void(T)>, NonCopyable {
public:
	template<typename ...Args>
	DelegateValue(Args ...args) :
		value(std::forward<Args>(args)...) {
	}

	virtual ~DelegateValue() = default;

	DelegateValue &operator=(T value) {
		this->value = value;
		Invoke(this->value);
		return *this;
	}

	/**
	 * Access the stored value.
	 * @return The value.
	 */
	operator const T &() const noexcept { return value; }

	const T &get() const { return value; }
	const T &operator*() const { return value; }
	const T *operator->() const { return &value; }

protected:
	T value;
};
}
