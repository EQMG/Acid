#include "UiObject.hpp"

#include "../devices/Display.hpp"
#include "../engine/Engine.hpp"
#include "../visual/DriverConstant.hpp"

namespace Flounder
{
	UiObject::UiObject(UiObject *parent, const Vector3 &position, const Vector3 &dimensions, const Vector2 &pivot) :
		m_parent(parent),
		m_children(new std::vector<UiObject*>()),
		m_visible(true),
		m_dimensions(new Vector3(dimensions)),
		m_position(new Vector3(position)),
		m_pivot(new Vector2(pivot)),
		m_scissor(new Vector4(0.0f, 0.0f, 1.0f, 1.0f)),
		m_positionOffset(new Vector2()),
		m_screenTransform(new Vector4()),
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

		delete m_positionOffset;
		delete m_screenTransform;

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
		m_alpha = Maths::Clamp(m_alpha, 0.0f, 1.0f);
		m_scale = m_scaleDriver->Update(Engine::Get()->GetDelta());

		if (IsVisible() && GetAlpha() != 0.0f)
		{
			UpdateObject();
		}

		const float da = m_dimensions->m_z == RelativeScreen ? Display::Get()->GetAspectRatio() : m_dimensions->m_z == RelativeInverse ? 1.0f / Display::Get()->GetAspectRatio() : 1.0f;
		const float dw = (m_dimensions->m_x / da) * m_scale;
		const float dh = m_dimensions->m_y * m_scale;

		const float pa = m_position->m_z == RelativeScreen ? 1.0f : Display::Get()->GetAspectRatio(); // TODO RelativeInverse
		const float px = (m_position->m_x / pa) - (dw * m_pivot->m_x) + m_positionOffset->m_x;
		const float py = m_position->m_y - (dh * -m_pivot->m_y) + m_positionOffset->m_y;

		m_screenTransform->Set(dw, dh, (2.0f * px) - 1.0f, (-2.0f * py) + 1.0f);
		
		/*const float dw = (m_dimensions->m_x / Display::Get()->GetAspectRatio()) * m_scale;
		const float dh = (m_dimensions->m_y) * m_scale;

		const float px = 0.5f;
		const float py = 0.5f;
		m_screenTransform->Set(dw, dh, (2.0f * px) - 1.0f, (-2.0f * py) + 1.0f);*/
	}

	void UiObject::UpdateObject()
	{
		// Pure virtual function.
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
