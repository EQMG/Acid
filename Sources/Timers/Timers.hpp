#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "Engine/Engine.hpp"
#include "Utils/Delegate.hpp"
#include "Maths/Time.hpp"

namespace acid {
class ACID_EXPORT Timer {
	friend class Timers;
public:
	Timer(const Time &interval, const std::optional<uint32_t> &repeat) :
		interval(interval),
		next(Time::Now() + interval),
		repeat(repeat) {
	}

	const Time &GetInterval() const { return interval; }
	const std::optional<uint32_t> &GetRepeat() const { return repeat; }
	bool IsDestroyed() const { return destroyed; }
	void Destroy() { destroyed = true; }
	Delegate<void()> &OnTick() { return onTick; };

private:
	Time interval;
	Time next;
	std::optional<uint32_t> repeat;
	bool destroyed = false;
	Delegate<void()> onTick;
};

/**
 * @brief Module used for timed events.
 */
class ACID_EXPORT Timers : public Module::Registrar<Timers, Module::Stage::Post> {
public:
	Timers();
	~Timers();

	void Update() override;

	template<typename ...Args>
	Timer *Once(const Time &delay, std::function<void()> &&function, Args ...args) {
		std::unique_lock<std::mutex> lock(mutex);
		auto instance = std::make_unique<Timer>(delay, 1);
		instance->onTick.Add(std::move(function), args...);
		timers.emplace_back(std::move(instance));
		condition.notify_all();
		return instance.get();
	}

	template<typename ...Args>
	Timer *Every(const Time &interval, std::function<void()> &&function, Args ...args) {
		std::unique_lock<std::mutex> lock(mutex);
		auto instance = std::make_unique<Timer>(interval, std::nullopt);
		instance->onTick.Add(std::move(function), args...);
		timers.emplace_back(std::move(instance));
		condition.notify_all();
		return instance.get();
	}

	template<typename ...Args>
	Timer *Repeat(const Time &interval, uint32_t repeat, std::function<void()> &&function, Args ...args) {
		std::unique_lock<std::mutex> lock(mutex);
		auto instance = std::make_unique<Timer>(interval, repeat);
		instance->onTick.Add(std::move(function), args...);
		timers.emplace_back(std::move(instance));
		condition.notify_all();
		return instance.get();
	}

private:
	void ThreadRun();

	std::vector<std::unique_ptr<Timer>> timers;

	std::atomic_bool stop = false;
	std::thread worker;

	std::mutex mutex;
	std::condition_variable condition;
};
}
