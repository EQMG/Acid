#include "UiObject.hpp"

#include "../devices/Display.hpp"
#include "../engine/Engine.hpp"
#include "../visual/DriverConstant.hpp"

namespace Flounder
{
	UiObject::UiObject(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, const Vector2 &pivot, const bool &inScreenCoords) :
		m_parent(parent),
		m_children(new std::vector<UiObject*>()),
		m_visible(true),
		m_scissor(new Vector4(-1.0f, -1.0f, -1.0f, -1.0f)),
		m_dimensions(new Vector2(dimensions)),
		m_position(new Vector2(position)),
		m_pivot(new Vector2(pivot)),
		m_inScreenCoords(inScreenCoords),
		m_screenDimensions(new Vector2()),
		m_screenPosition(new Vector2()),
		m_alphaDriver(new DriverConstant(1.0f)),
		m_alpha(1.0f),
		m_scaleDriver(new DriverConstant(1.0f)),
		m_scale(1.0f)
	{
		if (parent != nullptr)
		{
			parent->m_children->push_back(this);
		}
	}

	UiObject::~UiObject()
	{
		//	for (auto child : *m_children)
		//	{
		//		delete child;
		//	}

		if (m_parent != nullptr)
		{
			m_parent->RemoveChild(this);
		}

		delete m_children;

		delete m_scissor;
		delete m_position;
		delete m_dimensions;
		delete m_pivot;

		delete m_screenPosition;
		delete m_screenDimensions;

		delete m_alphaDriver;
		delete m_scaleDriver;
	}

	void UiObject::Update()
	{
		for (auto child : *m_children)
		{
			child->Update();
		}

		m_alpha = m_alphaDriver->Update(Engine::Get()->GetDelta());
		m_scale = m_scaleDriver->Update(Engine::Get()->GetDelta());

		if (IsVisible() && GetAlpha() != 0.0f)
		{
			UpdateObject();
		}

		const float aspect = m_inScreenCoords ? static_cast<float>(Display::Get()->GetAspectRatio()) : 1.0f;
		m_screenDimensions->Set(
			m_dimensions->m_x * m_scale * aspect,
			m_dimensions->m_y * m_scale
		);
		m_screenPosition->Set(
			m_position->m_x + (m_screenDimensions->m_x / 2.0f), // - (m_pivot->m_x * aspect),
			m_position->m_y + (m_screenDimensions->m_y / 2.0f)  // - (m_pivot->m_y)
		);
	}

	void UiObject::RemoveChild(UiObject *child)
	{
		for (auto it = m_children->begin(); it != m_children->end(); ++it)
		{
			if (*it == child)
			{
				m_children->erase(it);
				return;
			}
		}
	}

	std::vector<UiObject*> *UiObject::GetAll(std::vector<UiObject *> *list)
	{
		if (IsVisible())
		{
			list->push_back(this);

			for (auto child : *m_children)
			{
				child->GetAll(list);
			}
		}

		return list;
	}

	void UiObject::SetParent(UiObject *parent)
	{
		m_parent->RemoveChild(this);
		parent->m_children->push_back(this);
		m_parent = parent;
	}

	bool UiObject::IsVisible() const
	{
		if (m_parent != nullptr)
		{
			return m_visible && m_parent->IsVisible();
		}

		return m_visible;
	}

	void UiObject::SetAlphaDriver(IDriver *alphaDriver)
	{
		delete m_alphaDriver;
		m_alphaDriver = alphaDriver;
	}

	float UiObject::GetAlpha() const
	{
		if (m_parent != nullptr)
		{
			return m_alpha * m_parent->GetAlpha();
		}

		return m_alpha;
	}

	void UiObject::SetScaleDriver(IDriver *scaleDriver)
	{
		delete m_scaleDriver;
		m_scaleDriver = scaleDriver;
	}
}
