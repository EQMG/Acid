#pragma once

#include <cmath>

#include "Utils/NonCopyable.hpp"
#include "Maths/ElapsedTime.hpp"
#include "Maths/Time.hpp"
#include "Module.hpp"
#include "Log.hpp"
#include "App.hpp"

namespace acid {
class ACID_CORE_EXPORT Delta {
public:
	void Update() {
		currentFrameTime = Time::Now();
		change = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
	}

	Time currentFrameTime;
	Time lastFrameTime;
	Time change;
};

class ACID_CORE_EXPORT ChangePerSecond {
public:
	void Update(const Time &time) {
		valueTemp++;

		if (std::floor(time.AsSeconds()) > std::floor(valueTime.AsSeconds())) {
			value = valueTemp;
			valueTemp = 0;
		}

		valueTime = time;
	}

	uint32_t valueTemp = 0, value = 0;
	Time valueTime;
};

/**
 * @brief Main class for Acid, manages modules and updates. After creating your Engine object call {@link Engine#Run} to start.
 */
class ACID_CORE_EXPORT Engine : NonCopyable {
public:
	/**
	 * Gets the engines instance.
	 * @return The current engine instance.
	 */
	static Engine *Get() { return Instance; }

	/**
	 * Carries out the setup for basic engine components and the engine. Call {@link Engine#Run} after creating a instance.
	 * @param argv0 The first argument passed to main.
	 * @param moduleFilter A filter for blacklisting/whitelisting modules.
	 */
	explicit Engine(std::string argv0, ModuleFilter &&moduleFilter = {});
	~Engine();

	/**
	 * The update function for the updater.
	 * @return {@code EXIT_SUCCESS} or {@code EXIT_FAILURE}
	 */
	int32_t Run();

	/**
	 * Gets the first argument passed to main.
	 * @return The first argument passed to main.
	 */
	const std::string &GetArgv0() const { return argv0; };

	/**
	 * Gets the engine's version.
	 * @return The engine's version.
	 */
	const Version &GetVersion() const { return version; }

	/**
	 * Gets the current application.
	 * @return The renderer manager.
	 */
	App *GetApp() const { return app.get(); }

	/**
	 * Sets the current application to a new application.
	 * @param app The new application.
	 */
	void SetApp(std::unique_ptr<App> &&app) { this->app = std::move(app); }

	/**
	 * Gets the fps limit.
	 * @return The frame per second limit.
	 */
	float GetFpsLimit() const { return fpsLimit; }

	/**
	 * Sets the fps limit. -1 disables limits.
	 * @param fpsLimit The new frame per second limit.
	 */
	void SetFpsLimit(float fpsLimit) { this->fpsLimit = fpsLimit; }

	/**
	 * Gets if the engine is running.
	 * @return If the engine is running.
	 */
	bool IsRunning() const { return running; }

	/**
	 * Gets the delta (seconds) between updates.
	 * @return The delta between updates.
	 */
	const Time &GetDelta() const { return deltaUpdate.change; }

	/**
	 * Gets the delta (seconds) between renders.
	 * @return The delta between renders.
	 */
	const Time &GetDeltaRender() const { return deltaRender.change; }

	/**
	 * Gets the average UPS over a short interval.
	 * @return The updates per second.
	 */
	uint32_t GetUps() const { return ups.value; }

	/**
	 * Gets the average FPS over a short interval.
	 * @return The frames per second.
	 */
	uint32_t GetFps() const { return fps.value; }

	/**
	 * Requests the engine to stop the game-loop.
	 */
	void RequestClose() { running = false; }

private:
	void CreateModule(Module::TRegistryMap::const_iterator it, const ModuleFilter &filter);
	void DestroyModule(TypeId id);
	void UpdateStage(Module::Stage stage);

	inline static Engine *Instance = nullptr;

	std::string argv0;
	Version version;

	std::unique_ptr<App> app;

	std::map<TypeId, std::unique_ptr<Module>> modules;
	std::map<Module::Stage, std::vector<TypeId>> moduleStages;

	float fpsLimit;
	bool running;

	Delta deltaUpdate, deltaRender;
	ElapsedTime elapsedUpdate, elapsedRender;
	ChangePerSecond ups, fps;
};
}
