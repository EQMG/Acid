#pragma once

#include <string>
#include <vector>

#include "extension.h"
#include "module.h"
#include "updater.h"
#include "version.h"
#include "updaterglfw.h"

namespace flounder {
	/// <summary>
	/// A framework used for simplifying the creation of complicated Java applications. By using flexible Module loading and Extension injecting, it allows the engine to be used for Networking, Imaging, AIs, Games, and many more applications.
	/// Start off by creating a new Framework object in your main thread, using Extensions in the constructor. By using Extensions: Modules can be required and therefor loaded into the framework.
	/// Implementing interfaces like <seealso cref="Standard"/> with your extension can allow you do task specific things with your Extensions. After creating your Framework object call <seealso cref="#run()"/> to start.
	/// </summary>
	class framework
	{
	private:
		static framework *M_INSTANCE;

		std::string m_unlocalizedName;

		version *m_version;
		updater *m_updater;

		std::vector<module*> m_modules;
		std::vector<module*> m_overrides;
		bool m_initialized;
		bool m_running;
		bool m_error;
		int m_fpsLimit;

	public:
		static framework *get() {
			return M_INSTANCE;
		}

		/// <summary>
		/// Carries out the setup for basic framework components and the framework. Call <seealso cref="#run()"/> after creating a instance.
		/// </summary>
		/// <param name="unlocalizedName"> The name to be used when determining where the roaming save files are saved. </param>
		/// <param name="updater"> The definition for how the framework will run. </param>
		/// <param name="fpsLimit"> The limit to FPS, (-1 disables limits). </param>
		/// <param name="extensions"> The extensions to load for the framework. </param>
		framework() // const std::string &unlocalizedName, updater *updater, int fpsLimit, extension extensions[]
		{
			// Sets the static object to this new one.
			framework::M_INSTANCE = this;

			// Sets the instances name.
			this->m_unlocalizedName = "Dank Memes"; //  unlocalizedName;

			// Increment revision every fix for the minor version release. Minor version represents the build month. Major incremented every two years OR after major core framework rewrites.
			this->m_version = new version("03.07.13");

			// Sets the frameworks updater.
			this->m_updater = new updaterglfw(); // updater;

			// Sets up the module and overrides lists.
			this->m_modules = std::vector<module*>();
			this->m_overrides = std::vector<module*>();

			// Registers these modules as global, we do this as everyone loves these guys <3
			// registerModules(loadModule(FlounderLogger::typeid)); // TODO

			// Force registers the extensions, as the framework was null when they were constructed.
			//for (int i = 0; i < sizeof(extensions); i++)
			//{
			//	//	registerModule(loadModule(extensions[i].getModule())).registerExtension(extensions[i]); // TODO
			//}

			this->m_initialized = false;
			this->m_running = true;
			this->m_error = false;
			this->m_fpsLimit = 60; //  fpsLimit;
		}

		~framework() {

		}

		std::string framework::getUnlocalizedName()
		{
			return m_unlocalizedName;
		}

		version *framework::getVersion()
		{
			return m_version;
		}

		updater *framework::getUpdater()
		{
			return m_updater;
		}

		std::vector<module*> framework::getModules()
		{
			return m_modules;
		}

		std::vector<module*> framework::getOverrides()
		{
			return m_overrides;
		}

		float framework::getTimeOffset()
		{
			return m_updater->getTimeOffset();
		}

		void framework::setTimeOffset(float timeOffset)
		{
			m_updater->setTimeOffset(timeOffset);
		}

		float framework::getDelta()
		{
			return m_updater->getDelta();
		}

		float framework::getDeltaRender()
		{
			return m_updater->getDeltaRender();
		}

		float framework::getTimeSec()
		{
			return m_updater->getTimeSec();
		}

		float framework::getTimeMs()
		{
			return m_updater->getTimeMs();
		}

		bool framework::isInitialized()
		{
			return m_initialized;
		}

		void framework::setInitialized(bool initialized)
		{
			this->m_initialized = initialized;
		}

		bool framework::isRunning()
		{
			return m_running;
		}

		void framework::requestClose(bool error)
		{
			this->m_running = false;

			// A statement in case it was already true.
			if (error)
			{
				this->m_error = true;
			}
		}

		int framework::getFpsLimit()
		{
			return this->m_fpsLimit;
		}

		void framework::setFpsLimit(int fpsLimit)
		{
			this->m_fpsLimit = fpsLimit;
			this->m_updater->setFpsLimit(fpsLimit);
		}
	};
}
