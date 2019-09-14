#include "MainApp.hpp"

#include <Files/Files.hpp>
#include <Devices/Mouse.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>
#include "MainRenderer.hpp"
#include "Scenes/PlayerFps.hpp"
#include "Scenes/Scene1.hpp"
#include "Resources/Resources.hpp"

#if defined(ACID_RELOAD)

#include <Engine/cr.h>

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation)
{
	switch (operation)
	{
	case CR_LOAD:
		Log::Out("[Guest] Operation load: ", ctx->version, '\n');
		Engine::Get()->SetApp(std::make_unique<test::MainApp>());
		return 0;
	case CR_UNLOAD:
		Log::Out("[Guest] Operation unload: ", ctx->version, '\n');
		Engine::Get()->SetApp(nullptr);
		return 0;
	default:
		return 0;
	}
}
#else
int main(int argc, char **argv)
{
	using namespace test;

	// Creates the engine.
	auto engine = std::make_unique<Engine>(argv[0]);
	engine->SetApp(std::make_unique<MainApp>());

	// Runs the game loop.
	auto exitCode = engine->Run();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}
#endif

namespace test
{
MainApp::MainApp() :
	App("Editor Test", {1, 0, 0}),
	m_buttonFullscreen(Key::F11),
	m_buttonScreenshot(Key::F9),
	m_buttonExit(Key::Delete)
{
	Log::Out("[Game] Constructor\n");

	// Registers file search paths.
	Log::Out("Working Directory: ", std::filesystem::current_path(), '\n');
	Files::Get()->AddSearchPath("Resources/Engine");

	m_buttonFullscreen.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
		}
	});
	m_buttonScreenshot.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Resources::Get()->GetThreadPool().Enqueue([]()
			{
				Graphics::Get()->CaptureScreenshot(Time::GetDateTime("Screenshots/%Y%m%d%H%M%S.png"));
			});
		}
	});
	m_buttonExit.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Engine::Get()->RequestClose();
		}
	});

	// Registers modules.
	//Engine::Get()->RemoveModule<Shadows>();

	// Registers components.
	auto &componentRegister = Scenes::Get()->GetComponentRegister();
	componentRegister.Add<PlayerFps>("playerFps");

	// Sets values to modules.
	//Window::Get()->SetTitle("Test Physics");
	//Window::Get()->SetIcons({ "Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png",
	//	"Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png" });
	//Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
	//Renderer::Get()->SetManager(std::make_unique<MainRenderer>());
	Scenes::Get()->SetScene(std::make_unique<Scene1>());
}

MainApp::~MainApp()
{
	Log::Out("[Game] Destructor\n");
	//Files::Get()->ClearSearchPath();

	// Deregisters components.
	auto &componentRegister = Scenes::Get()->GetComponentRegister();
	componentRegister.Remove("playerFps");

	//Renderer::Get()->SetManager(nullptr);
	Scenes::Get()->SetScene(nullptr);
	Uis::Get()->GetCanvas().ClearChildren();
}

void MainApp::Update()
{
}
}
