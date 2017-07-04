#pragma once

#include <string>
#include <vector>

#include "extension.h"
#include "module.h"
#include "updater.h"
#include "version.h"

#include "maths.h"

/// <summary>
/// A framework used for simplifying the creation of complicated Java applications. By using flexible Module loading and Extension injecting, it allows the engine to be used for Networking, Imaging, AIs, Games, and many more applications.
/// Start off by creating a new Framework object in your main thread, using Extensions in the constructor. By using Extensions: Modules can be required and therefor loaded into the framework.
/// Implementing interfaces like <seealso cref="Standard"/> with your extension can allow you do task specific things with your Extensions. After creating your Framework object call <seealso cref="#run()"/> to start.
/// </summary>
class Framework
{
private:
	static Framework *M_INSTANCE;

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
	/// <summary>
	/// Carries out the setup for basic framework components and the framework. Call <seealso cref="#run()"/> after creating a instance.
	/// </summary>
	/// <param name="unlocalizedName"> The name to be used when determining where the roaming save files are saved. </param>
	/// <param name="updater"> The definition for how the framework will run. </param>
	/// <param name="fpsLimit"> The limit to FPS, (-1 disables limits). </param>
	/// <param name="extensions"> The extensions to load for the framework. </param>
	Framework(const std::string &unlocalizedName, updater *updater, int fpsLimit, extension extensions[])
	{		
		// Sets the static object to this new one.
		Framework::M_INSTANCE = this;

		// Sets the instances name.
		this->m_unlocalizedName = unlocalizedName;

		// Increment revision every fix for the minor version release. Minor version represents the build month. Major incremented every two years OR after major core framework rewrites.
		this->m_version = new version("03.07.13");

		// Sets the frameworks updater.
		this->m_updater = updater;

		// Sets up the module and overrides lists.
		this->m_modules = std::vector<module*>();
		this->m_overrides = std::vector<module*>();
		
		// Registers these modules as global, we do this as everyone loves these guys <3
		// registerModules(loadModule(FlounderLogger::typeid)); // TODO

		// Force registers the extensions, as the framework was null when they were constructed.
		for (int i = 0; i < sizeof(extensions); i++)
		{
		//	registerModule(loadModule(extensions[i].getModule())).registerExtension(extensions[i]); // TODO
		}

		this->m_initialized = false;
		this->m_running = true;
		this->m_error = false;
		this->m_fpsLimit = fpsLimit;
	}
};
