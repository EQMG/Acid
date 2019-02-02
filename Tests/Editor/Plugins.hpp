#pragma once

#include <Engine/Engine.hpp>
#include <Files/FileWatcher.hpp>

using namespace acid;

struct cr_plugin;

namespace test
{
	/// <summary>
	/// A module used for managing the world.
	/// </summary>
	class Plugins :
		public Module
	{
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Plugins *Get() { return Engine::Get()->GetModuleManager().Get<Plugins>(); }

		Plugins();

		~Plugins();

		void Update() override;
	private:
		std::string m_loadedPath;
		FileWatcher m_watcher;
		std::unique_ptr<cr_plugin> m_plugin;
		bool m_update;
	};
}