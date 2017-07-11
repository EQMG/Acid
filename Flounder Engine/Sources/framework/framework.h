#pragma once

#include "imodule.h"
#include "iupdater.h"

/// <summary>
/// The base Flounder folder.
/// </summary>
namespace flounder {
	/// <summary>
	/// A framework used for simplifying the creation of complicated Java applications. By using flexible Module loading and Extension injecting, it allows the engine to be used for Networking, Imaging, AIs, Games, and many more applications.
	/// Start off by creating a new Framework object in your main thread, using Extensions in the constructor. By using Extensions: Modules can be required and therefor loaded into the framework.
	/// Implementing interfaces like <seealso cref="Standard"/> with your extension can allow you do task specific things with your Extensions. After creating your Framework object call <seealso cref="#run()"/> to start.
	/// </summary>
	class framework
	{
	private:
		static framework *G_INSTANCE;

		std::string m_unlocalizedName;

		bool m_initialized;
		bool m_running;
		bool m_error;
		int m_fpsLimit;

		iupdater *m_updater;
	public:
		static framework *get() 
		{
			return G_INSTANCE;
		}

		/// <summary>
		/// Carries out the setup for basic framework components and the framework. Call <seealso cref="#run()"/> after creating a instance.
		/// </summary>
		/// <param name="unlocalizedName"> The name to be used when determining where the roaming save files are saved. </param>
		/// <param name="fpsLimit"> The limit to FPS, (-1 disables limits). </param>
		framework(const std::string &unlocalizedName, int fpsLimit);

		~framework();

		void loadUpdater(iupdater* iupdater);

		void init();

		void run();

		imodule *getInstance(const std::string &name);

		std::string framework::getUnlocalizedName();

		inline double framework::getTimeOffset() { return m_updater->getTimeOffset(); }

		inline void framework::setTimeOffset(const double timeOffset) { m_updater->setTimeOffset(timeOffset); }

		inline double framework::getDelta() { return m_updater->getDelta(); }

		inline double framework::getDeltaRender() { return m_updater->getDeltaRender(); }

		inline double framework::getTimeSec() { return m_updater->getTimeSec(); }

		inline double framework::getTimeMs() { return m_updater->getTimeMs(); }

		bool framework::isInitialized();

		void framework::setInitialized(const bool initialized);

		bool framework::isRunning();

		void framework::requestClose(const bool error);

		int framework::getFpsLimit();

		void framework::setFpsLimit(const int fpsLimit);
	};
}
