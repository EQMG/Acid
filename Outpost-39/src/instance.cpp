#include "instance.h"

instance::instance()
{
	m_buttonFullscreen = new buttonkeyboard(1, GLFW_KEY_F11);
	m_buttonWireframe = new buttonkeyboard(1, GLFW_KEY_P);
	m_buttonExit = new buttonkeyboard(1, GLFW_KEY_DELETE);
	
	audio::add(new sound("music", "res/handFarts.wav"));
	sound *object = audio::get("music");
	if (object != NULL)
	{
		std::cerr << "Could not find music!" << std::endl;
		object->loop();
	}

	m_time = 0.0f;

	m_terrainDay = new entity(vector2(0.5f, 0.5f), vector2(3.41333f, 1.0f), texture::newTexture()->setFile("res/game/terrainDay.png")->create(), 1);
	m_terrainDay->addComponent([&](entity *object)
	{
		m_terrainDay->setAlpha(1.0f - cos(m_time / 30.0f));
	});
	entities::get()->add(m_terrainDay);

	m_terrainNight = new entity(vector2(0.5f, 0.5f), vector2(3.41333f, 1.0f), texture::newTexture()->setFile("res/game/terrainNight.png")->create(), 1);
	m_terrainNight->addComponent([&](entity *object)
	{
		m_terrainNight->setAlpha(cos(m_time / 30.0f));
	});
	entities::get()->add(m_terrainNight);

	m_dome = new entity(vector2(0.5f, 0.4f), vector2(1.41241f, 1.0f), texture::newTexture()->setFile("res/game/dome.png")->create(), 1);
	m_dome->addComponent([&](entity *object)
	{
	});
	entities::get()->add(m_dome);

	m_ais = std::vector<aiplayer*>();
	m_ais.push_back(new aiplayer(texture::newTexture()->setFile("res/game/player1.png")->setNumberOfRows(2)->create(), "decaxon"));
}

instance::~instance()
{
	delete m_buttonFullscreen;
	delete m_buttonWireframe;
	delete m_buttonExit;
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

	if (m_buttonExit->wasDown())
	{
		framework::get()->requestClose(false);
	}
#endif

	m_time += framework::get()->getDelta();

	for (aiplayer *ai : m_ais)
	{
		if (m_terrainDay->getAlpha() < 0.1f)
		{
			if (!ai->isAdventuring())
			{
				if (ai->isAtCentre())
				{
					if (ai->getLevel() == 3)
					{
						if (!ai->isSleeping())
						{
							const float beds[4] = { 0.1f, 0.3f, 0.7f, 0.8f };
							ai->setBedTarget(beds[rand() % 4 + 1]);
						}
					}
					else
					{
						ai->setLevel(3);
					}
				}
				else
				{
					ai->goToCentre();
				}
			}
		}
	}
}
