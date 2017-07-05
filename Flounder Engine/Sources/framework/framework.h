#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "module.h"

#include "../maths/maths.h"

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

	public:
		static framework *get() {
			return G_INSTANCE;
		}

		/// <summary>
		/// Carries out the setup for basic framework components and the framework. Call <seealso cref="#run()"/> after creating a instance.
		/// </summary>
		/// <param name="unlocalizedName"> The name to be used when determining where the roaming save files are saved. </param>
		/// <param name="fpsLimit"> The limit to FPS, (-1 disables limits). </param>
		framework(const std::string &unlocalizedName, int fpsLimit)
		{
			// Sets the static object to this new one.
			framework::G_INSTANCE = this;

			// Sets the instances name.
			m_unlocalizedName = unlocalizedName;

			// Registers these modules as global, we do this as everyone loves these guys <3
			// registerModules(loadModule(FlounderLogger::typeid)); // TODO

			// Force registers the extensions, as the framework was null when they were constructed.
			//for (int i = 0; i < sizeof(extensions); i++)
			//{
			//	//	registerModule(loadModule(extensions[i].getModule())).registerExtension(extensions[i]); // TODO
			//}

			m_initialized = false;
			m_running = true;
			m_error = false;
			m_fpsLimit = fpsLimit;
		}

		~framework() {
		}

		void run() {
		}

		module *getInstance(std::string name) {
			return NULL;
		}

		std::string framework::getUnlocalizedName()
		{
			return m_unlocalizedName;
		}

		float framework::getTimeOffset()
		{
			return 0.0f; //  updater::getTimeOffset();
		}

		void framework::setTimeOffset(float timeOffset)
		{
			// updater::setTimeOffset(timeOffset);
		}

		float framework::getDelta()
		{
			return 0.0f; // updater::getDelta();
		}

		float framework::getDeltaRender()
		{
			return 0.0f; // updater::getDeltaRender();
		}

		float framework::getTimeSec()
		{
			return 0.0f; // updater::getTimeSec();
		}

		float framework::getTimeMs()
		{
			return 0.0f; // updater::getTimeMs();
		}

		bool framework::isInitialized()
		{
			return m_initialized;
		}

		void framework::setInitialized(bool initialized)
		{
			m_initialized = initialized;
		}

		bool framework::isRunning()
		{
			return m_running;
		}

		void framework::requestClose(bool error)
		{
			m_running = false;

			// A statement in case it was already true.
			if (error)
			{
				m_error = true;
			}
		}

		int framework::getFpsLimit()
		{
			return m_fpsLimit;
		}

		void framework::setFpsLimit(int fpsLimit)
		{
			m_fpsLimit = fpsLimit;
		}
	};
}
