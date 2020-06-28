#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <rocket/rocket.hpp>

#include "Engine/Engine.hpp"
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
	rocket::signal<void()> &OnTick() { return onTick; };

private:
	Time interval;
	Time next;
	std::optional<uint32_t> repeat;
	bool destroyed = false;
	rocket::signal<void()> onTick;
};

/**
 * @brief Module used for timed events.
 */
class ACID_EXPORT Timers : public Module::Registrar<Timers> {
	inline static const bool Registered = Register(Stage::Post);
public:
	Timers();
	~Timers();

	void Update() override;

	template<class Instance>
	Timer *Once(Instance *object, std::function<void()> &&function, const Time &delay) {
		std::unique_lock<std::mutex> lock(mutex);
		auto instance = std::make_unique<Timer>(delay, 1);
		instance->onTick.connect(object, std::move(function));
		timers.emplace_back(std::move(instance));
		condition.notify_all();
		return instance.get();
	}

	template<class Instance>
	Timer *Every(Instance *object, std::function<void()> &&function, const Time &interval) {
		std::unique_lock<std::mutex> lock(mutex);
		auto instance = std::make_unique<Timer>(interval, std::nullopt);
		instance->onTick.connect(object, std::move(function));
		timers.emplace_back(std::move(instance));
		condition.notify_all();
		return instance.get();
	}

	template<class Instance>
	Timer *Repeat(Instance *object, std::function<void()> &&function, const Time &interval, uint32_t repeat) {
		std::unique_lock<std::mutex> lock(mutex);
		auto instance = std::make_unique<Timer>(interval, repeat);
		instance->onTick.connect(object, std::move(function));
		timers.emplace_back(std::move(instance));
		condition.notify_all();
		return instance.get();
	}

	Timer *Once(std::function<void()> &&function, const Time &delay) {
		std::unique_lock<std::mutex> lock(mutex);
		auto instance = std::make_unique<Timer>(delay, 1);
		instance->onTick.connect(std::move(function));
		timers.emplace_back(std::move(instance));
		condition.notify_all();
		return instance.get();
	}

	Timer *Every(std::function<void()> &&function, const Time &interval) {
		std::unique_lock<std::mutex> lock(mutex);
		auto instance = std::make_unique<Timer>(interval, std::nullopt);
		instance->onTick.connect(std::move(function));
		timers.emplace_back(std::move(instance));
		condition.notify_all();
		return instance.get();
	}

	Timer *Repeat(std::function<void()> &&function, const Time &interval, uint32_t repeat) {
		std::unique_lock<std::mutex> lock(mutex);
		auto instance = std::make_unique<Timer>(interval, repeat);
		instance->onTick.connect(std::move(function));
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
