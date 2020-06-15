#include "Engine.hpp"

#include "Config.hpp"

namespace acid {
Engine *Engine::Instance = nullptr;

Engine::Engine(std::string argv0, ModuleFilter &&moduleFilter) :
	argv0(std::move(argv0)),
	version{ACID_VERSION_MAJOR, ACID_VERSION_MINOR, ACID_VERSION_PATCH},
	fpsLimit(-1.0f),
	running(true),
	elapsedUpdate(15.77ms),
	elapsedRender(-1s) {
	Instance = this;
	Log::OpenLog(Time::GetDateTime("Logs/%Y%m%d%H%M%S.txt"));

#if defined(ACID_DEBUG)
	Log::Out("Version: ", ACID_VERSION, '\n');
	Log::Out("Git: ", ACID_COMPILED_COMMIT_HASH, " on ", ACID_COMPILED_BRANCH, '\n');
	Log::Out("Compiled on: ", ACID_COMPILED_SYSTEM, " from: ", ACID_COMPILED_GENERATOR, " with: ", ACID_COMPILED_COMPILER, "\n\n");
#endif

	// TODO: Optimize and clean up!
	std::vector<TypeId> created;
	for (const auto &[moduleId, moduleTest] : Module::Registry()) {
		for (const auto &requireId : moduleTest.requires) {
			const auto &requireTest = Module::Registry()[requireId];
			if (std::find(created.begin(), created.end(), requireId) == created.end() &&
				moduleFilter.Check(requireId)) {
				auto &&module = Module::Registry()[requireId].create();
				modules.emplace(Module::StageIndex(requireTest.stage, requireId), std::move(module));
				created.emplace_back(requireId);
				Log::Debug("Module created: ", requireId, "\n");
			} else if (!moduleFilter.Check(requireId)) {
				Log::Error("Module depends on another module this is excluded!");
			}
		}

		if (std::find(created.begin(), created.end(), moduleId) == created.end() &&
			moduleFilter.Check(moduleId)) {
			auto &&module = moduleTest.create();
			modules.emplace(Module::StageIndex(moduleTest.stage, moduleId), std::move(module));
			created.emplace_back(moduleId);
		}
	}
}

Engine::~Engine() {
	Module::Registry().clear();
	Log::CloseLog();
}

int32_t Engine::Run() {
	while (running) {
		if (app) {
			if (!app->started) {
				app->Start();
				app->started = true;
			}
			
			app->Update();
		}

		elapsedRender.SetInterval(Time::Seconds(1.0f / fpsLimit));

		// Always-Update.
		UpdateStage(Module::Stage::Always);

		if (elapsedUpdate.GetElapsed() != 0) {
			// Resets the timer.
			ups.Update(Time::Now());

			// Pre-Update.
			UpdateStage(Module::Stage::Pre);

			// Update.
			UpdateStage(Module::Stage::Normal);

			// Post-Update.
			UpdateStage(Module::Stage::Post);

			// Updates the engines delta.
			deltaUpdate.Update();
		}

		// Prioritize updates over rendering.
		//if (!Maths::AlmostEqual(elapsedUpdate.GetInterval().AsSeconds(), deltaUpdate.change.AsSeconds(), 0.8f)) {
		//	continue;
		//}

		// Renders when needed.
		if (elapsedRender.GetElapsed() != 0) {
			// Resets the timer.
			fps.Update(Time::Now());

			// Render
			UpdateStage(Module::Stage::Render);

			// Updates the render delta, and render time extension.
			deltaRender.Update();
		}
	}

	return EXIT_SUCCESS;
}

void Engine::UpdateStage(Module::Stage stage) {
	for (auto &[stageIndex, module] : modules) {
		if (stageIndex.first == stage)
			module->Update();
	}
}
}
