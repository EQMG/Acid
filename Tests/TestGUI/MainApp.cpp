#include "MainApp.hpp"

#include <Files/Files.hpp>
#include <Devices/Mouse.hpp>
#include <Graphics/Graphics.hpp>
#include <Resources/Resources.hpp>
#include <Scenes/Scenes.hpp>
#include "MainRenderer.hpp"
#include "Scenes/Scene1.hpp"
#include "Graphics/Pipelines/PipelineCompute.hpp"

int main(int argc, char **argv) {
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

namespace test {
MainApp::MainApp() :
	App("Test GUI", {1, 0, 0}),
	m_fileObserver(std::filesystem::current_path(), 2s),
	m_buttonFullscreen(Key::F11),
	m_buttonScreenshot(Key::F9),
	m_buttonExit(Key::Delete) {
	// Registers file search paths.
	Log::Out("Working Directory: ", std::filesystem::current_path(), '\n');
	Files::Get()->AddSearchPath("Resources/Engine");

	// Watches all files in the working directory.
	m_fileObserver.OnChange().Add([this](std::filesystem::path path, FileObserver::Status status) {
		switch (status) {
		case FileObserver::Status::Created:
			Log::Out("Created ", path, '\n');
			break;
		case FileObserver::Status::Modified:
			Log::Out("Modified ", path, '\n');
			break;
		case FileObserver::Status::Erased:
			Log::Out("Erased ", path, '\n');
			break;
		}
	});

	m_buttonFullscreen.OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
		}
	});
	m_buttonScreenshot.OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			Resources::Get()->GetThreadPool().Enqueue([]() {
				Graphics::Get()->CaptureScreenshot(Time::GetDateTime("Screenshots/%Y%m%d%H%M%S.png"));
			});
		}
	});
	m_buttonExit.OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			Engine::Get()->RequestClose();
		}
	});
}

MainApp::~MainApp() {
	Files::Get()->ClearSearchPath();

	Graphics::Get()->SetRenderer(nullptr);
	Scenes::Get()->SetScene(nullptr);
}

std::unique_ptr<Image2d> ComputeBRDF(uint32_t size) {
	auto brdfImage = std::make_unique<Image2d>(Vector2ui(size), VK_FORMAT_R16G16_SFLOAT, VK_IMAGE_LAYOUT_GENERAL);

	// Creates the pipeline.
	CommandBuffer commandBuffer(true, VK_QUEUE_COMPUTE_BIT);
	PipelineCompute compute("Shaders/Brdf.comp");

	// Bind the pipeline.
	compute.BindPipeline(commandBuffer);

	// Updates descriptors.
	DescriptorsHandler descriptorSet(compute);
	descriptorSet.Push("outColour", brdfImage.get());
	descriptorSet.Update(compute);

	// Runs the compute pipeline.
	descriptorSet.BindDescriptor(commandBuffer, compute);
	compute.CmdRender(commandBuffer, brdfImage->GetExtent());
	commandBuffer.SubmitIdle();

#if defined(ACID_DEBUG)
	// Saves the BRDF Image.
	auto bmp = brdfImage->GetBitmap();
	bmp.Write("Deferred/Brdf.png");
#endif

	return brdfImage;
}

void MainApp::Start() {
	// Sets values to modules.
	Window::Get()->SetTitle("Test GUI");
	Window::Get()->SetIcons({
		"Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png",
		"Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png"
		});
	//Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
	Graphics::Get()->SetRenderer(std::make_unique<MainRenderer>());
	Scenes::Get()->SetScene(std::make_unique<Scene1>());
	ComputeBRDF(512);
}

void MainApp::Update() {
}
}
