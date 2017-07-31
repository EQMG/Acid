#include "instance.h"

instance::instance()
{
	m_buttonFullscreen = new buttonkeyboard(1, GLFW_KEY_F11);
	m_buttonWireframe = new buttonkeyboard(1, GLFW_KEY_P);
	m_buttonLockMouse = new buttonkeyboard(1, GLFW_KEY_M);
	m_buttonExit = new buttonkeyboard(1, GLFW_KEY_DELETE);
	
	audio::add(new sound("music", "res/ld39Outpost.wav"));
	sound *object = audio::get("music");
	if (object != NULL)
	{
		object->loop();
		object->setGain(0.1f);
	}


	std::vector<particletype*> *types = new std::vector<particletype*>();
	types->push_back(new particletype("blue", texture::newTexture()->setFile("res/particles/blueParticle.png")->create(), 10.6f, 0.10f));
	//	types->push_back(new particletype("green", texture::newTexture()->setFile("res/particles/greenParticle.png")->create(), 10.6f, 0.50f));
	//	types->push_back(new particletype("purple", texture::newTexture()->setFile("res/particles/purpleParticle.png")->create(), 10.6f, 0.50f));
	//	types->push_back(new particletype("red", texture::newTexture()->setFile("res/particles/redParticle.png")->create(), 10.6f, 0.50f));
	//	types->push_back(new particletype("yellow", texture::newTexture()->setFile("res/particles/yellowParticle.png")->create(), 10.6f, 0.50f));
	m_systemTest = new particlesystem(types, new spawnpoint(), 1000, 0.01f, -0.01f);
	m_systemTest->setSystemCentre(vector3(0.0f, 0.0f, 0.0f));
	particles::get()->addSystem(m_systemTest);
}

instance::~instance()
{
	delete m_buttonFullscreen;
	delete m_buttonWireframe;
	delete m_buttonLockMouse;
	delete m_buttonExit;

	delete m_systemTest;
}

void instance::update()
{
#ifndef FLOUNDER_PLATFORM_WEB
	if (m_buttonFullscreen->wasDown())
	{
		display::get()->setFullscreen(!display::get()->isFullscreen());
	}

	if (m_buttonWireframe->wasDown())
	{
		renderer::get()->goWireframe(!renderer::get()->isInWireframe());
	}

	if (m_buttonLockMouse->wasDown())
	{
		mouse::get()->setCursorHidden(!mouse::get()->isCursorDisabled());
	}

	if (m_buttonExit->wasDown())
	{
		framework::get()->requestClose(false);
	}
#endif
}
