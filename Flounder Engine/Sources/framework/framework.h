#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "module.h"
#include "updater.h"

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

		updater *m_updater;

	public:
		static framework *get() {
			return G_INSTANCE;
		}

		/// <summary>
		/// Carries out the setup for basic framework components and the framework. Call <seealso cref="#run()"/> after creating a instance.
		/// </summary>
		/// <param name="unlocalizedName"> The name to be used when determining where the roaming save files are saved. </param>
		/// <param name="fpsLimit"> The limit to FPS, (-1 disables limits). </param>
		framework(const std::string &unlocalizedName, int fpsLimit);

		~framework();

		void run(updater* updater);

		module *getInstance(std::string name);

		std::string framework::getUnlocalizedName();

		float framework::getTimeOffset();

		void framework::setTimeOffset(float timeOffset);

		float framework::getDelta();

		float framework::getDeltaRender();

		float framework::getTimeSec();

		float framework::getTimeMs();

		bool framework::isInitialized();

		void framework::setInitialized(bool initialized);

		bool framework::isRunning();

		void framework::requestClose(bool error);

		int framework::getFpsLimit();

		void framework::setFpsLimit(int fpsLimit);
	};
}
