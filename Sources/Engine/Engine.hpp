#pragma once

#include <chrono>
#include <memory>
#include "ModuleRegister.hpp"
#include "ModuleUpdater.hpp"

/// <summary>
/// The base Acid namespace.
/// </summary>
namespace acid
{
	/// <summary>
	/// Main class for Acid, manages modules and updates. After creating your Engine object call <seealso cref="#Run()"/> to start.
	/// </summary>
	class ACID_EXPORT Engine
	{
	private:
		typedef std::chrono::high_resolution_clock HighResolutionClock;
		typedef std::chrono::duration<float, std::milli> MillisecondsType;

		static Engine *INSTANCE;

		std::chrono::time_point<HighResolutionClock> m_start;
		float m_timeOffset;

		ModuleRegister m_moduleRegister;
		ModuleUpdater m_moduleUpdater;

		float m_fpsLimit;

		bool m_initialized;
		bool m_running;
		bool m_error;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current engine instance. </returns>
		static Engine *Get() { return INSTANCE; }

		/// <summary>
		/// Carries out the setup for basic engine components and the engine. Call <seealso cref="#run()"/> after creating a instance.
		/// </summary>
		/// <param name="emptyRegister"> If the module register will start empty. </param>
		Engine(const bool &emptyRegister = false);

		~Engine();

		/// <summary>
		/// The update function for the updater.
		/// </summary>
		/// <returns> EXIT_SUCCESS or EXIT_FAILURE. </returns>
		int32_t Run();

		/// <summary>
		/// Gets a module instance by type.
		/// </summary>
		/// <param name="T"> The module type to find. </param>
		/// <returns> The found module. </returns>
		template<typename T>
		T *GetModule() const { return m_moduleRegister.GetModule<T>(); }

		/// <summary>
		/// Registers a module with the register.
		/// </summary>
		/// <param name="module"> The modules object. </param>
		/// <param name="update"> The modules update type. </param>
		/// <returns> The registered module. </returns>
		IModule *RegisterModule(IModule *module, const ModuleUpdate &update) { return m_moduleRegister.RegisterModule(module, update); }

		/// <summary>
		/// Registers a module with the register.
		/// </summary>
		/// <param name="update"> The modules update type. </param>
		/// <param name="T"> The type of module to register. </param>
		/// <returns> The registered module. </returns>
		template<typename T>
		T *RegisterModule(const ModuleUpdate &update) { return m_moduleRegister.RegisterModule<T>(update); }

		/// <summary>
		/// Deregisters a module.
		/// </summary>
		/// <param name="module"> The module to deregister. </param>
		/// <returns> If the module was deregistered. </returns>
		bool DeregisterModule(IModule *module) { return m_moduleRegister.DeregisterModule(module); }

		/// <summary>
		/// Deregisters a module.
		/// </summary>
		/// <param name="T"> The type of module to deregister. </param>
		/// <returns> If the module was deregistered. </returns>
		template<typename T>
		bool DeregisterModule() { return m_moduleRegister.DeregisterModule<T>(); }

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
		/// Gets the fps limit.
		/// </summary>
		/// <returns> The fps limit. </returns>
		float GetFpsLimit() const { return m_fpsLimit; }

		/// <summary>
		/// Sets the fps limit. -1 disables limits.
		/// </summary>
		/// <param name="fpsLimit"> The new fps limit. </param>
		void SetFpsLimit(const float &fpsLimit) { m_fpsLimit = fpsLimit; }

		/// <summary>
		/// Gets the delta (seconds) between updates.
		/// </summary>
		/// <returns> The delta between updates. </returns>
		float GetDelta() const { return m_moduleUpdater.GetDelta(); }

		/// <summary>
		/// Gets the delta (seconds) between renders.
		/// </summary>
		/// <returns> The delta between renders. </returns>
		float GetDeltaRender() const { return m_moduleUpdater.GetDeltaRender(); }

		/// <summary>
		/// Gets the current time of the engine instance.
		/// </summary>
		/// <returns> The current engine time in milliseconds. </returns>
		float GetTimeMs() const;

		/// <summary>
		/// Gets the current time of the engine instance.
		/// </summary>
		/// <returns> The current engine time in seconds. </returns>
		float GetTime() const { return GetTimeMs() / 1000.0f; }

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

		/// <summary>
		/// Gets the current date time as a string. "%d-%m-%Y %I:%M:%S"
		/// </summary>
		/// <returns> The date time as a string. </returns>
		static std::string GetDateTime();
	};
}
