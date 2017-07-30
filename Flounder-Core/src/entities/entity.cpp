#include "entity.h"

namespace flounder
{
	entity::entity(const vector2 &position, const vector2 &dimensions, texture *texture, const int &selectedRow)
	{
		m_components = new std::vector<std::function<void(entity *)>>();

		m_visible = true;
		m_position = new vector2(position);
		m_dimensions = new vector2(dimensions);
		m_meshSize = new vector2(0.5f, 0.5f);

		m_inScreenCoords = true;

		m_screenPosition = new vector2();
		m_screenDimensions = new vector2();

		m_texture = texture;
		m_flipX = false;
		m_flipY = false;
		m_selectedRow = 0;

		m_textureOffset = new vector2();
		m_colourOffset = new colour();

		m_rotation = 0.0f;
		m_alpha = 1.0f;
		m_scale = 1.0f;
	}

	entity::~entity()
	{
		delete m_components;

		delete m_position;
		delete m_dimensions;
		delete m_meshSize;

		delete m_screenPosition;
		delete m_screenDimensions;

		delete m_texture;

		delete m_textureOffset;
		delete m_colourOffset;
	}

	void entity::update()
	{
		for (std::function<void(entity *)> f : *m_components)
		{
			f(this);
		}

		m_screenPosition->set(
			m_position->m_x * (m_inScreenCoords ? static_cast<float>(display::get()->getAspectRatio()) : 1.0f),
			m_position->m_y
		);
		m_screenDimensions->set(m_dimensions->m_x, m_dimensions->m_y);
		m_screenDimensions->scale(m_scale);

		int numberOfRows = m_texture != NULL ? m_texture->getNumberOfRows() : 1;
		int column = m_selectedRow % numberOfRows;
		int row = m_selectedRow / numberOfRows;
		m_textureOffset->set(static_cast<float>(column) / static_cast<float>(numberOfRows), static_cast<float>(row) / static_cast<float>(numberOfRows));
	}

	void entity::addComponent(std::function<void(entity *)> component)
	{
		m_components->push_back(component);
	}
}
