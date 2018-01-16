#pragma once

#include <chrono>
#include "IModule.hpp"
#include "IUpdater.hpp"

/// <summary>
/// The base Flounder folder.
/// </summary>
namespace Flounder
{
	/// <summary>
	/// A engine used for simplifying the creation of complicated applications. By using flexible Module loading and Extension injecting, it allows the engine to be used for Networking, Imaging, AIs, Games, and many more applications.
	/// Start off by creating a new Engine object in your main thread, using Extensions in the constructor. By using Extensions: Modules can be required and therefor loaded into the engine.
	/// Implementing interfaces like <seealso cref="Standards"/> with your extension can allow you do task specific things with your Extensions. After creating your Engine object call <seealso cref="#Run()"/> to start.
	/// </summary>
	class Engine
	{
	private:
		typedef std::chrono::high_resolution_clock HighResolutionClock;
		typedef std::chrono::duration<float, std::milli> MillisecondsType;

		static Engine *g_instance;

		std::chrono::time_point<HighResolutionClock> m_start;
		float m_timeOffset;

		bool m_initialized;
		bool m_running;
		bool m_error;

		IUpdater *m_updater;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current engine instance. </returns>
		static Engine *Get()
		{
			return g_instance;
		}

		/// <summary>
		/// Carries out the setup for basic engine components and the engine. Call <seealso cref="#run()"/> after creating a instance.
		/// </summary>
		Engine();

		/// <summary>
		/// Deconstructor for the engine.
		/// </summary>
		~Engine();

		/// <summary>
		/// Loads the updater into the engine.
		/// </summary>
		/// <param name="updater"> The updater. </param>
		void SetUpdater(IUpdater *updater);

		/// <summary>
		/// The update function for the updater.
		/// </summary>
		/// <returns> EXIT_SUCCESS/EXIT_FAILURE. </returns>
		int Run() const;

		/// <summary>
		/// Gets a module instance by name.
		/// </summary>
		/// <param name="name"> The module name to find. </param>
		/// <returns> The found module. </returns>
		IModule *GetModule(const std::string &name) const { return m_updater->GetModule(name); }

		/// <summary>
		/// Gets the added/removed time for the engine (seconds).
		/// </summary>
		/// <returns> The time offset. </returns>
		float GetTimeOffset() const { return m_timeOffset; }

		/// <summary>
		/// Sets the time offset for the engine (seconds).
		/// </summary>
		/// <param name="timeOffset"> The new time offset. </param>
		void SetTimeOffset(const float &timeOffset) { m_timeOffset = timeOffset; }

		/// <summary>
		/// Gets the delta (seconds) between updates.
		/// </summary>
		/// <returns> The delta between updates. </returns>
		float GetDelta() const { return m_updater->GetDelta(); }

		/// <summary>
		/// Gets the delta (seconds) between renders.
		/// </summary>
		/// <returns> The delta between renders. </returns>
		float GetDeltaRender() const { return m_updater->GetDeltaRender(); }

		/// <summary>
		/// Gets the current time of the engine instance.
		/// </summary>
		/// <returns> The current engine time in seconds. </returns>
		float GetTime() const { return GetTimeMs() / 1000.0f; }

		/// <summary>
		/// Gets the current time of the engine instance.
		/// </summary>
		/// <returns> The current engine time in milliseconds. </returns>
		float GetTimeMs() const { return std::chrono::duration_cast<MillisecondsType>(HighResolutionClock::now() - m_start).count() + (m_timeOffset / 1000.0f); }

		/// <summary>
		/// Gets if the engine has been initialized.
		/// </summary>
		/// <returns> If the engine has been initialized. </returns>
		bool IsInitialized() const { return m_initialized; }

		/// <summary>
		/// Sets if the engine has been initialized.
		/// </summary>
		/// <param name="initialized"> If the engine has been initialized. </param>
		void SetInitialized(const bool &initialized) { m_initialized = initialized; }

		/// <summary>
		/// Gets if the engine is running.
		/// </summary>
		/// <returns> If the engine is running. </returns>
		bool IsRunning() const { return m_running; }

		/// <summary>
		/// Requests the engine to delete and stop the gameloop.
		/// </summary>
		/// <param name="error"> If a bad error occured. </param>
		void RequestClose(const bool &error);
	};
}
