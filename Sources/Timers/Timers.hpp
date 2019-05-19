#pragma once

#include <mutex>
#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"
#include "Maths/Time.hpp"

namespace acid
{
class ACID_EXPORT TimerInstance
{
public:
	TimerInstance(const Time &interval, const std::optional<uint32_t> &repeat) :
		m_interval(interval),
		m_next(Time::Now() + m_interval),
		m_repeat(repeat),
		m_destroyed(false)
	{
	}

	const Time &GetInterval() const { return m_interval; }

	const std::optional<uint32_t> &GetRepeat() const { return m_repeat; }

	const bool &IsDestroyed() const { return m_destroyed; }

	void Destroy() { m_destroyed = true; }

	Delegate<void(void)> &OnTick() { return m_onTick; };

private:
	friend class Timers;

	Time m_interval;
	Time m_next;
	std::optional<uint32_t> m_repeat;
	bool m_destroyed;
	Delegate<void()> m_onTick;
};

/**
 * @brief Module used for timed events.
 */
class ACID_EXPORT Timers :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Timers *Get() { return Engine::Get()->GetModule<Timers>(); }

	Timers();

	~Timers();

	void Update() override;

	template<typename ...Args>
	TimerInstance *Once(const Time &delay, std::function<void(void)> &&function, Args ...args)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		auto instance = std::make_unique<TimerInstance>(delay, 1);
		instance->m_onTick.Add(std::move(function), args...);
		m_timers.emplace_back(std::move(instance));
		m_condition.notify_all();
		return instance.get();
	}

	template<typename ...Args>
	TimerInstance *Every(const Time &interval, std::function<void(void)> &&function, Args ...args)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		auto instance = std::make_unique<TimerInstance>(interval, std::nullopt);
		instance->m_onTick.Add(std::move(function), args...);
		m_timers.emplace_back(std::move(instance));
		m_condition.notify_all();
		return instance.get();
	}

	template<typename ...Args>
	TimerInstance *Repeat(const Time &interval, const uint32_t &repeat, std::function<void(void)> &&function, Args ...args)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		auto instance = std::make_unique<TimerInstance>(interval, repeat);
		instance->m_onTick.Add(std::move(function), args...);
		m_timers.emplace_back(std::move(instance));
		m_condition.notify_all();
		return instance.get();
	}

private:
	void Run();

	std::vector<std::unique_ptr<TimerInstance>> m_timers;

	bool m_stop;
	std::thread m_worker;

	std::mutex m_mutex;
	std::condition_variable m_condition;
};
}
