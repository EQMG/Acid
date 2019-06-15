#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Maths/ElapsedTime.hpp"
#include "Maths/Time.hpp"
#include "ModuleHolder.hpp"
#include "Game.hpp"

namespace acid
{
class ACID_EXPORT Delta
{
public:
	void Update()
	{
		m_currentFrameTime = Time::Now();
		m_change = m_currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = m_currentFrameTime;
	}

	Time m_currentFrameTime;
	Time m_lastFrameTime;
	Time m_change;
};

class ACID_EXPORT ChangePerSecond
{
public:
	void Update(const Time &time)
	{
		m_valueTemp++;

		if (std::floor(time.AsSeconds()) > std::floor(m_valueTime.AsSeconds()))
		{
			m_value = m_valueTemp;
			m_valueTemp = 0;
		}

		m_valueTime = time;
	}

	uint32_t m_valueTemp{}, m_value{};
	Time m_valueTime;
};

/**
 * @brief Main class for Acid, manages modules and updates. After creating your Engine object call {@link Engine#Run} to start.
 */
class ACID_EXPORT Engine :
	public NonCopyable
{
public:
	struct Version
	{
		uint32_t m_major, m_minor, m_patch;
	};

	/**
	 * Gets the engines instance.
	 * @return The current engine instance.
	 */
	static Engine *Get() { return INSTANCE; }

	/**
	 * Carries out the setup for basic engine components and the engine. Call {@link Engine#Run} after creating a instance.
	 * @param argv0 The first argument passed to main.
	 * @param emptyRegister If the module register will start empty.
	 */
	explicit Engine(std::string argv0, const bool &emptyRegister = false);

	/**
	 * The update function for the updater.
	 * @return {@code EXIT_SUCCESS} or {@code EXIT_FAILURE}
	 */
	int32_t Run();

	/**
	 * Gets the first argument passed to main.
	 * @return The first argument passed to main.
	 */
	const std::string &GetArgv0() const { return m_argv0; };

	/**
	 * Gets the engine's version.
	 * @return The engine's version.
	 */
	const Version &GetEngineVersion() const { return m_engineVersion; }

	/**
	 * Gets the game's version.
	 * @return The game's version.
	 */
	const Version &GetGameVersion() const { return m_gameVersion; }

	/**
	 * Sets the games version, for driver support.
	 * @param T The game version.
	 */
	void SetGameVersion(const Version &gameVersion) { m_gameVersion = gameVersion; }

	/**
	 * Gets the game's name.
	 * @return The game's name.
	 */
	const std::string &GetGameName() const { return m_gameName; }

	/**
	 * Sets the game's name, for driver support.
	 * @param T The gam'e name.
	 */
	void SetGameName(const std::string &gameName) { m_gameName = gameName; }

	/**
	 * Checks whether a Module exists or not.
	 * @tparam T The Module type.
	 * @return If the Module has the System.
	 */
	template<typename T>
	bool HasModule() const
	{
		return m_modules.Has<T>();
	}

	/**
	 * Gets a module instance by type from the register.
	 * @tparam T The Module type.
	 * @return The Module.
	 */
	template<typename T>
	T *GetModule() const
	{
		return m_modules.Get<T>();
	}

	/**
	 * Adds a Module.
	 * @tparam T The Module type.
	 * @param stage The Module stage.
	 * @tparam Args The constructor arg types.
	 * @param args The constructor arguments.
	 */
	template<typename T, typename... Args>
	void AddModule(const Module::Stage& stage, Args &&...args)
	{
		m_modules.Add<T>(stage, std::make_unique<T>(std::forward<Args>(args)...));
	}

	/**
	 * Removes a Module.
	 * @tparam T The Module type.
	 */
	template<typename T>
	void RemoveModule()
	{
		m_modules.Remove<T>();
	}

	/**
	 * Gets the current game.
	 * @return The renderer manager.
	 */
	Game *GetGame() const { return m_game.get(); }

	/**
	 * Sets the current game to a new game.
	 * @param game The new game.
	 */
	void SetGame(std::unique_ptr<Game> &&game) { m_game = std::move(game); }

	/**
	 * Gets the fps limit.
	 * @return The frame per second limit.
	 */
	const float &GetFpsLimit() const { return m_fpsLimit; }

	/**
	 * Sets the fps limit. -1 disables limits.
	 * @param fpsLimit The new frame per second limit.
	 */
	void SetFpsLimit(const float &fpsLimit) { m_fpsLimit = fpsLimit; }

	/**
	 * Gets if the engine is running.
	 * @return If the engine is running.
	 */
	const bool &IsRunning() const { return m_running; }

	/**
	 * Gets the delta (seconds) between updates.
	 * @return The delta between updates.
	 */
	const Time &GetDelta() const { return m_deltaUpdate.m_change; }

	/**
	 * Gets the delta (seconds) between renders.
	 * @return The delta between renders.
	 */
	const Time &GetDeltaRender() const { return m_deltaRender.m_change; }

	/**
	 * Gets the average UPS over a short interval.
	 * @return The updates per second.
	 */
	const uint32_t &GetUps() const { return m_ups.m_value; }

	/**
	 * Gets the average FPS over a short interval.
	 * @return The frames per second.
	 */
	const uint32_t &GetFps() const { return m_fps.m_value; }

	/**
	 * Requests the engine to stop the game-loop.
	 */
	void RequestClose() { m_running = false; }

private:
	ACID_STATE static Engine *INSTANCE;

	std::string m_argv0;
	Version m_engineVersion;
	Version m_gameVersion;
	std::string m_gameName;

	ModuleHolder m_modules;
	std::unique_ptr<Game> m_game;

	float m_fpsLimit;
	bool m_running;

	Delta m_deltaUpdate;
	Delta m_deltaRender;
	ElapsedTime m_elapsedUpdate;
	ElapsedTime m_elapsedRender;

	ChangePerSecond m_ups, m_fps;
};
}
