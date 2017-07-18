#pragma once

#include "imodule.h"
#include "iupdater.h"

#ifdef FLOUNDER_PLATFORM_WEB
#include <functional>
#include <emscripten/emscripten.h>

static void dispatch_main(void* fp)
{
	std::function<void()>* func = (std::function<void()>*)fp;
	(*func)();
}
#endif

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

		bool m_initialized;
		bool m_running;
		bool m_error;

		iupdater *m_updater;
	public:
		static framework *get() 
		{
			return G_INSTANCE;
		}

		/// <summary>
		/// Carries out the setup for basic framework components and the framework. Call <seealso cref="#run()"/> after creating a instance.
		/// </summary>
		framework();

		~framework();

		void loadUpdater(iupdater* iupdater);

		void init();

		void run();

		imodule *getInstance(const std::string &name);

		inline double getTimeOffset() { return m_updater->getTimeOffset(); }

		inline void setTimeOffset(const double &timeOffset) { m_updater->setTimeOffset(timeOffset); }

		inline double getDelta() { return m_updater->getDelta(); }

		inline double getDeltaRender() { return m_updater->getDeltaRender(); }

		inline double getTimeSec() { return m_updater->getTimeSec(); }

		inline double getTimeMs() { return m_updater->getTimeMs(); }

		inline bool isInitialized() { return m_initialized; }

		inline void setInitialized(const bool &initialized) { m_initialized = initialized; }

		inline bool isRunning() { return m_running; }

		void requestClose(const bool &error);
	};
}
