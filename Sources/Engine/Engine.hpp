#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Maths/Time.hpp"
#include "ModuleHolder.hpp"
#include "ModuleUpdater.hpp"
#include "Log.hpp"
#include "Game.hpp"

namespace acid
{
/**
 * @brief Main class for Acid, manages modules and updates. After creating your Engine object call {@link Engine#Run} to start.
 */
class ACID_EXPORT Engine :
	public NonCopyable
{
public:
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
	 * Gets the module holder used by the engine instance.
	 * @return The engines module holder.
	 */
	ModuleHolder &GetModuleHolder() { return m_moduleHolder; }

	/**
	 * Gets the current game.
	 * @return The renderer manager.
	 */
	Game *GetGame() const { return m_game.get(); }

	/**
	 * Sets the current game to a new game.
	 * @param game The new game.
	 */
	void SetGame(Game *game) { m_game.reset(game); }

	/**
	 * Gets the current time of the engine instance.
	 * @return The current engine time.
	 */
	static Time GetTime();

	/**
	 * Gets the current date time as a string. "%d-%m-%Y %I:%M:%S"
	 * @return The date time as a string.
	 */
	static std::string GetDateTime();

	/**
	 * Gets the first argument passed to main.
	 * @return The first argument passed to main.
	 */
	const std::string &GetArgv0() const { return m_argv0; };

	/**
	 * Gets the added/removed time for the engine.
	 * @return The time offset.
	 */
	const Time &GetTimeOffset() const { return m_timeOffset; }

	/**
	 * Sets the time offset for the engine.
	 * @param timeOffset The new time offset.
	 */
	void SetTimeOffset(const Time &timeOffset) { m_timeOffset = timeOffset; }

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
	const Time &GetDelta() const { return m_moduleUpdater.GetDelta(); }

	/**
	 * Gets the delta (seconds) between renders.
	 * @return The delta between renders.
	 */
	const Time &GetDeltaRender() const { return m_moduleUpdater.GetDeltaRender(); }

	/**
	 * Gets the average UPS over a short interval.
	 * @return The updates per second.
	 */
	const uint32_t &GetUps() const { return m_moduleUpdater.GetUps(); }

	/**
	 * Gets the average FPS over a short interval.
	 * @return The frames per second.
	 */
	const uint32_t &GetFps() const { return m_moduleUpdater.GetFps(); }

	/**
	 * Requests the engine to delete and stop the game-loop.
	 * @param error If a bad error occurred.
	 */
	void RequestClose(const bool &error);

private:
	static ACID_STATE Engine *INSTANCE;

	ModuleHolder m_moduleHolder;
	ModuleUpdater m_moduleUpdater;

	std::unique_ptr<Game> m_game;

	std::string m_argv0;
	Time m_timeOffset;
	float m_fpsLimit;
	bool m_running;
	bool m_error;
};
}
