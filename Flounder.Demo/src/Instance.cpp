#include "Instance.hpp"

#include "entities/EntityTest.hpp"

Instance::Instance()
{
	m_buttonFullscreen = new ButtonKeyboard({ GLFW_KEY_F11 });
	m_buttonLockMouse = new ButtonKeyboard({ GLFW_KEY_M });
	m_buttonExit = new ButtonKeyboard({ GLFW_KEY_DELETE });

	/*audio::add(new sound("music", "res/ld39Outpost.wav"));
	sound *object = audio::get("music");
	
	if (object != nullptr)
	{
		object->loop();
		object->setGain(0.1f);
	}*/

	/*std::vector<particletype*> *types = new std::vector<particletype*>();
	types->push_back(new particletype("blue", texture::newTexture()->setFile("res/particles/blueParticle.png")->setNumberOfRows(4)->create(), 10.6f, 0.3f));
	types->push_back(new particletype("green", texture::newTexture()->setFile("res/particles/greenParticle.png")->setNumberOfRows(4)->create(), 10.6f, 0.3f));
	types->push_back(new particletype("purple", texture::newTexture()->setFile("res/particles/purpleParticle.png")->setNumberOfRows(4)->create(), 10.6f, 0.3f));
	types->push_back(new particletype("red", texture::newTexture()->setFile("res/particles/redParticle.png")->setNumberOfRows(4)->create(), 10.6f, 0.3f));
	types->push_back(new particletype("yellow", texture::newTexture()->setFile("res/particles/yellowParticle.png")->setNumberOfRows(4)->create(), 10.6f, 0.3f));
	m_systemTest = new particlesystem(types, new spawncircle(30.0f, Vector3(0.0f, 1.0f, 0.0f)), 1000, 1.0f, -0.1f);
	m_systemTest->setSystemCentre(Vector3(0.0f, 0.0f, 0.0f));
	particles::get()->addSystem(m_systemTest);*/

	// EntityTest *test = new EntityTest(nullptr, Vector3(), Vector3());
	// componentalpha *meme = test->getComponent<componentalpha*>();

	Model *modelTest = new Model("res/test/model.obj");
	Transform *transformTest = new Transform(Vector3(100.892f, -10.0f, -256.0f), Vector3(0.0f, 90.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
	Matrix4 *matrixWorld = transformTest->GetWorldMatrix(nullptr);

	Colour colourHex("#cc6600");
	std::cout << colourHex.m_r << ", " << colourHex.m_g << ", " << colourHex.m_b << std::endl;
	std::cout << Colour::GetHex(colourHex) << std::endl;

	// float degrees = Maths::Degrees(2.0f * PI);

	delete matrixWorld;
	delete transformTest;
	delete modelTest;
}

Instance::~Instance()
{
	delete m_buttonFullscreen;
	delete m_buttonLockMouse;
	delete m_buttonExit;

	delete m_systemTest;
}

void Instance::Update()
{
	if (m_buttonFullscreen->WasDown())
	{
		Display::Get()->SetFullscreen(!Display::Get()->IsFullscreen());
	}

	if (m_buttonLockMouse->WasDown())
	{
		Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorDisabled());
	}

	if (m_buttonExit->WasDown())
	{
		Engine::Get()->RequestClose(false);
	}
}
