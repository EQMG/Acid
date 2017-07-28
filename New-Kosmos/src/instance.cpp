#include "instance.h"

instance::instance()
{
	m_buttonFullscreen = new buttonkeyboard(1, GLFW_KEY_F11);
	m_buttonWireframe = new buttonkeyboard(1, GLFW_KEY_P);
	m_buttonExit = new buttonkeyboard(1, GLFW_KEY_DELETE);

	std::vector<particletype*> *types = new std::vector<particletype*>();
	types->push_back(new particletype("blue", texture::newTexture()->setFile("res/particles/blueParticle.png")->create(), 10.6f, 0.10f));
	types->push_back(new particletype("green", texture::newTexture()->setFile("res/particles/greenParticle.png")->create(), 10.6f, 0.10f));
	types->push_back(new particletype("purple", texture::newTexture()->setFile("res/particles/purpleParticle.png")->create(), 10.6f, 0.10f));
	types->push_back(new particletype("red", texture::newTexture()->setFile("res/particles/redParticle.png")->create(), 10.6f, 0.10f));
	types->push_back(new particletype("yellow", texture::newTexture()->setFile("res/particles/yellowParticle.png")->create(), 10.6f, 0.10f));
	m_systemTest = new particlesystem(types, new spawnpoint(), 100, 0.01f, 0.0f);
	particles::get()->addSystem(m_systemTest);
}

instance::~instance()
{
	delete m_buttonFullscreen;
	delete m_buttonWireframe;
	delete m_buttonExit;

	delete m_systemTest;
}

void instance::update()
{
	if (m_buttonFullscreen->wasDown())
	{
		display::get()->setFullscreen(!display::get()->isFullscreen());
	}

	if (m_buttonWireframe->wasDown())
	{
		renderer::get()->goWireframe(!renderer::get()->isInWireframe());
	}

	if (m_buttonExit->wasDown())
	{
		framework::get()->requestClose(false);
	}
}
