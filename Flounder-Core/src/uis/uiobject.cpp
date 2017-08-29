#include "uiobject.hpp"

namespace flounder
{
	uiobject::uiobject(uiobject *parent, const vector2 &position, const vector2 &dimensions)
	{
		if (parent != nullptr)
		{
			parent->m_children->push_back(this);
		}

		m_visible = true;
		m_parent = parent;
		m_children = new std::vector<uiobject*>();

		m_position = new vector2(position);
		m_dimensions = new vector2(dimensions);
		m_meshSize = new vector2();
		m_scissor = new vector4(-1.0f, -1.0f, -1.0f, -1.0f);

		m_inScreenCoords = true;

		m_screenPosition = new vector2();
		m_screenDimensions = new vector2();
		m_positionOffsets = new vector2();

		m_rotationDriver = new driverconstant(0.0f);
		m_rotation = 0.0f;

		m_alphaDriver = new driverconstant(1.0f);
		m_alpha = 1.0f;

		m_scaleDriver = new driverconstant(1.0f);
		m_scale = 1.0f;
	}

	uiobject::~uiobject()
	{
		//	for (screenobject *child : *m_children)
		//	{
		//		delete child;
		//	}

		if (m_parent != nullptr)
		{
			m_parent->removeChild(this);
		}

		delete m_children;

		delete m_position;
		delete m_dimensions;
		delete m_meshSize;
		delete m_scissor;

		delete m_screenPosition;
		delete m_screenDimensions;
		delete m_positionOffsets;

		delete m_rotationDriver;
		delete m_alphaDriver;
		delete m_scaleDriver;
	}

	void uiobject::update()
	{
		for (uiobject *child : *m_children)
		{
			child->update();
		}

		m_rotation = m_rotationDriver->update(framework::get()->getDelta());
		m_alpha = m_alphaDriver->update(framework::get()->getDelta());
		m_scale = m_scaleDriver->update(framework::get()->getDelta());

		if (isVisible() && getAlpha() != 0.0f)
		{
			updateObject();
		}

		m_screenPosition->set(
			m_position->m_x * (m_inScreenCoords ? static_cast<float>(display::get()->getAspectRatio()) : 1.0f) + m_positionOffsets->m_x,
			m_position->m_y + m_positionOffsets->m_y
		);
		m_screenDimensions->set(m_dimensions->m_x, m_dimensions->m_y);
		m_screenDimensions->scale(m_scale);
	}

	void uiobject::removeChild(uiobject *child)
	{
		for (std::vector<uiobject*>::iterator it = m_children->begin(); it != m_children->end(); ++it)
		{
			if (*it == child)
			{
				m_children->erase(it);
				return;
			}
		}
	}

	std::vector<uiobject*> *uiobject::getAll(std::vector<uiobject *> *list)
	{
		if (isVisible())
		{
			list->push_back(this);

			for (uiobject *child : *m_children)
			{
				child->getAll(list);
			}
		}

		return list;
	}

	void uiobject::setParent(uiobject *parent)
	{
		m_parent->removeChild(this);
		parent->m_children->push_back(this);
		m_parent = parent;
	}

	bool uiobject::isVisible()
	{
		if (m_parent != nullptr)
		{
			return m_visible && m_parent->isVisible();
		}
		else
		{
			return m_visible;
		}
	}

	void uiobject::setRotationDriver(idriver *rotationDriver)
	{
		delete m_rotationDriver;
		m_rotationDriver = rotationDriver;
	}

	void uiobject::setAlphaDriver(idriver *alphaDriver)
	{
		delete m_alphaDriver;
		m_alphaDriver = alphaDriver;
	}

	float uiobject::getAlpha()
	{
		if (m_parent != nullptr)
		{
			return m_alpha * m_parent->getAlpha();
		}
		else
		{
			return m_alpha;
		}
	}

	void uiobject::setScaleDriver(idriver *scaleDriver)
	{
		delete m_scaleDriver;
		m_scaleDriver = scaleDriver;
	}
}
