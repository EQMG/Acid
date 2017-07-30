#include "instance.h"
#include "ais/aitaskadventure.h"

const float instance::DAY_LENGTH = 30.0f;

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

	m_terrainDay = new entity(vector2(0.5f, 0.5f), vector2(3.41333f, 1.0f), texture::newTexture()->setFile("res/game/terrainDay.png")->create(), 1);
	m_terrainDay->addComponent([&](entity *object)
	{
		m_terrainDay->setAlpha(cos(framework::get()->getTimeSec() / DAY_LENGTH));
	});
	entities::get()->add(m_terrainDay);

	m_terrainNight = new entity(vector2(0.5f, 0.5f), vector2(3.41333f, 1.0f), texture::newTexture()->setFile("res/game/terrainNight.png")->create(), 1);
	m_terrainNight->addComponent([&](entity *object)
	{
		m_terrainNight->setAlpha(1.0f - cos(framework::get()->getTimeSec() / DAY_LENGTH));
	});
	entities::get()->add(m_terrainNight);

	m_dome = new entity(vector2(0.5f, 0.4f), vector2(1.41241f, 1.0f), texture::newTexture()->setFile("res/game/dome.png")->create(), 1);
	m_dome->addComponent([&](entity *object)
	{
	});
	entities::get()->add(m_dome);

	m_ais = std::vector<aiplayer*>();
	aiplayer *decaxon = new aiplayer(texture::newTexture()->setFile("res/game/player1.png")->setNumberOfRows(2)->create(), "decaxon");
	decaxon->addTask(new aitasklevel(decaxon->getEntity(), 0));
	decaxon->addTask(new aitasktargetx(decaxon->getEntity(), 0.0f));
	m_ais.push_back(decaxon);

	m_timerFortune = new timer(30.0f);
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

	for (aiplayer *ai : m_ais)
	{
		if (m_terrainDay->getAlpha() < 0.1f)
		{
			if (!ai->containsTask<aitaskadventure>() && !ai->containsTask<aitasksleep>())
			{
				ai->addTask(new aitasktargetx(ai->getEntity(), 0.5f));
				ai->addTask(new aitasklevel(ai->getEntity(), 3));
				ai->addTask(new aitasktargetx(ai->getEntity(), maths::randomInRange(0.38f, 0.62f)));
				ai->addTask(new aitasksleep(ai->getEntity()));
			}
		}
	}

	if (m_timerFortune->isPassedTime())
	{
		float luck = maths::randomInRange(-1.0f, 1.0f);

		// Bad luck ;(
		if (luck < 0.0f)
		{
			std::cout << "Unlucky fortune!" << std::endl;
		}
		// Good luck :)
		else
		{
			std::cout << "Lucky fortune!" << std::endl;
		}

		m_timerFortune->setInterval(maths::randomInRange(30.0f, 60.0f));
		m_timerFortune->resetStartTime();
	}
}
