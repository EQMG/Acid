#pragma once

#include <map>
#include <stdlib.h>
#include "../Devices/Display.hpp"
#include "../Engine/IUpdater.hpp"
#include "../Maths/Delta.hpp"
#include "../Maths/Timer.hpp"

namespace Flounder
{
	/// <summary>
	/// The default GLFW updater for the engine.
	/// </summary>
	class PlatformUpdater :
		public IUpdater
	{
	private:
		Delta *m_deltaUpdate;
		Delta *m_deltaRender;
		Timer *m_timerUpdate;
		Timer *m_timerRender;

		std::multimap<float, std::pair<std::string, IModule *>> *m_modules;
	public:
		PlatformUpdater();

		~PlatformUpdater();

		void Create() override;

		void Update() override;

		template<typename T>
		void ModuleCreate(ModuleUpdate typeUpdate, std::string moduleName);

		void AddModule(const ModuleUpdate &typeUpdate, std::string moduleName, IModule *module) override;

		IModule *GetModule(const std::string &name) override;

		float GetDelta() override { return m_deltaUpdate->GetChange(); };

		float GetDeltaRender() override { return m_deltaRender->GetChange(); };
	private:
		void RunUpdate(const ModuleUpdate &typeUpdate) const;
	};

	template<typename T>
	void PlatformUpdater::ModuleCreate(ModuleUpdate typeUpdate, std::string moduleName)
	{
		T *module = static_cast<T *>(malloc(sizeof(T)));
		AddModule(typeUpdate, moduleName, module);
		new(module) T();
	}
}
