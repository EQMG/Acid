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
	TimerInstance(const Time &intervel, const std::optional<uint32_t> &repeat) :
		m_start(Engine::Get()->GetTime()),
		m_intervel(intervel),
		m_repeat(repeat),
		m_destroyed(false)
	{
	}

	const Time &GetIntervel() const { return m_intervel; }

	const std::optional<uint32_t> &GetRepeat() const { return m_repeat; }

	bool IsFinished()
	{
		return m_repeat && *m_repeat == 0;
	}

	const bool &IsDestroyed() const { return m_destroyed; }

	void Destroy() { m_destroyed = true; }

	Delegate<void(void)> &OnTick() { return m_onTick; };

private:
	friend class Timers;

	Time m_start;
	Time m_intervel;
	std::optional<uint32_t> m_repeat;
	bool m_destroyed;
	Delegate<void(void)> m_onTick;
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
	TimerInstance *Once(const Time &intervel, std::function<void(void)> &&function, Args ...args)
	{
		auto &instance = std::make_unique<TimerInstance>(intervel, 1);
		instance->m_onTick.Add(std::move(function), args...);
		m_timers.emplace_back(std::move(instance));
		return instance.get();
	}

	template<typename ...Args>
	TimerInstance *Every(const Time &intervel, std::function<void(void)> &&function, Args ...args)
	{
		auto &instance = std::make_unique<TimerInstance>(intervel, std::nullopt);
		instance->m_onTick.Add(std::move(function), args...);
		m_timers.emplace_back(std::move(instance));
		return instance.get();
	}

	template<typename ...Args>
	TimerInstance *Repeat(const Time &intervel, const uint32_t &repeat, std::function<void(void)> &&function, Args ...args)
	{
		auto &instance = std::make_unique<TimerInstance>(intervel, repeat);
		instance->m_onTick.Add(std::move(function), args...);
		m_timers.emplace_back(std::move(instance));
		return instance.get();
	}

private:
	std::vector<std::unique_ptr<TimerInstance>> m_timers;
};
}
