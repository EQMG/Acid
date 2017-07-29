#include "instance.h"

instance::playerai::playerai(texture *texture, const std::string &name)
{
	m_entity = new entity(vector2(0.0f, 0.5f), vector2(0.08f, 0.08f), texture, 1);
	m_entity->addComponent([&](entity *object)
	{
		if (m_trackCentre)
		{
			traceX(0.5f);

			if (atCentre())
			{
				m_trackCentre = false;
			}

			// Walk anim.
			object->setSelectedRow(1);
		}
		else if (m_sleeping)
		{
			if (!atBed())
			{
				//	if (m_bedXPos != -1.0f)
				//	{
				traceX(m_bedXPos);
				//	}
			}
			else
			{
				// m_bedXPos = -1.0f;
				// GO to seeping state.
				object->setSelectedRow(4);
			}
		}
	});
	entities::get()->add(m_entity);
	setLevel(2);

	m_name = name;
	m_trackCentre = false;
	m_sleeping = false;
	m_bedXPos = -1.0f;
	m_adventuting = false;
	m_dead = false;
}

instance::playerai::~playerai()
{
	delete m_entity;
}

void instance::playerai::traceX(const float &x)
{
	bool left = (m_entity->getPosition()->m_x - x) <= 0.0f;
	const float track_rate = 0.07f;

	if (left)
	{
		m_entity->getPosition()->m_x += track_rate * framework::get()->getDelta();
		m_entity->setFlipTexture(true);
	}
	else
	{
		m_entity->getPosition()->m_x -= track_rate * framework::get()->getDelta();
		m_entity->setFlipTexture(false);
	}
}

instance::instance()
{
	m_buttonFullscreen = new buttonkeyboard(1, GLFW_KEY_F11);
	m_buttonWireframe = new buttonkeyboard(1, GLFW_KEY_P);
	m_buttonExit = new buttonkeyboard(1, GLFW_KEY_DELETE);

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

	m_ais = std::vector<playerai*>();
	m_ais.push_back(new playerai(texture::newTexture()->setFile("res/game/player1.png")->setNumberOfRows(2)->create(), "meme"));
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

	if (m_terrainDay->getAlpha() < 0.1f)
	{
		for (playerai *ai : m_ais)
		{
			if (!ai->m_adventuting)
			{
				if (ai->atCentre())
				{
					if (ai->getLevel() == 3)
					{
						if (!ai->isSleeping())
						{
							const float beds[4] = {0.1f, 0.3f, 0.7f, 0.8f};
							ai->setBed(beds[rand() % 4 + 1]);
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
