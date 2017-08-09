#pragma once

#include "imodule.h"
#include "iupdater.h"

/// <summary>
/// The base Flounder folder.
/// </summary>
namespace flounder
{
	/// <summary>
	/// A framework used for simplifying the creation of complicated Java applications. By using flexible Module loading and Extension injecting, it allows the engine to be used for Networking, Imaging, AIs, Games, and many more applications.
	/// Start off by creating a new Framework object in your main thread, using Extensions in the constructor. By using Extensions: Modules can be required and therefor loaded into the framework.
	/// Implementing interfaces like <seealso cref="standards"/> with your extension can allow you do task specific things with your Extensions. After creating your Framework object call <seealso cref="#run()"/> to start.
	/// </summary>
	class framework
	{
	private:
		static framework *G_INSTANCE;

		bool m_initialized;
		bool m_running;
		bool m_error;

		iupdater *m_updater;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current framework instance. </returns>
		static framework *get()
		{
			return G_INSTANCE;
		}

		/// <summary>
		/// Carries out the setup for basic framework components and the framework. Call <seealso cref="#run()"/> after creating a instance.
		/// </summary>
		framework();

		/// <summary>
		/// Deconstructor for the framework.
		/// </summary>
		~framework();

		/// <summary>
		/// Loads the updater into the framework.
		/// </summary>
		/// <param name="updater"> The updater. </param>
		void setUpdater(iupdater *updater);

		/// <summary>
		/// The update function for the updater.
		/// </summary>
		void run();

		/// <summary>
		/// Gets a module instance by name.
		/// </summary>
		/// <param name="name"> The module name to find. </param>
		/// <returns> The found module. </returns>
		imodule *getInstance(const std::string &name);

		/// <summary>
		/// Gets the added/removed time for the framework (seconds).
		/// </summary>
		/// <returns> The time offset. </returns>
		inline float getTimeOffset() const { return m_updater->getTimeOffset(); }

		/// <summary>
		/// Sets the time offset for the framework (seconds).
		/// </summary>
		/// <param name="timeOffset"> The new time offset. </param>
		inline void setTimeOffset(const float &timeOffset) const { m_updater->setTimeOffset(timeOffset); }

		/// <summary>
		/// Gets the delta (seconds) between updates.
		/// </summary>
		/// <returns> The delta between updates. </returns>
		inline float getDelta() const { return m_updater->getDelta(); }

		/// <summary>
		/// Gets the delta (seconds) between renders.
		/// </summary>
		/// <returns> The delta between renders. </returns>
		inline float getDeltaRender() const { return m_updater->getDeltaRender(); }

		/// <summary>
		/// Gets the current time of the framework instance.
		/// </summary>
		/// <returns> The current framework time in seconds. </returns>
		inline float getTimeSec() const { return m_updater->getTimeSec(); }

		/// <summary>
		/// Gets the current time of the framework instance.
		/// </summary>
		/// <returns> The current framework time in milliseconds. </returns>
		inline float getTimeMs() const { return m_updater->getTimeMs(); }

		/// <summary>
		/// Gets if the framework has been initialized.
		/// </summary>
		/// <returns> If the framework has been initialized. </returns>
		inline bool isInitialized() const { return m_initialized; }

		/// <summary>
		/// Sets if the framework has been initialized.
		/// </summary>
		/// <param name="initialized"> If the framework has been initialized. </param>
		inline void setInitialized(const bool &initialized) { m_initialized = initialized; }

		/// <summary>
		/// Gets if the framework is running.
		/// </summary>
		/// <returns> If the framework is running. </returns>
		inline bool isRunning() const { return m_running; }

		/// <summary>
		/// Requests the framework to delete and stop the gameloop.
		/// </summary>
		/// <param name="error"> If a bad error occured. </param>
		void requestClose(const bool &error);
	};
}
