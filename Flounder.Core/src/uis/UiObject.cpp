﻿#include "UiObject.hpp"

#include "../devices/Display.hpp"
#include "../engine/Engine.hpp"
#include "../visual/DriverConstant.hpp"

namespace Flounder
{
	UiObject::UiObject(UiObject *parent, const UiBound &rectangle) :
		m_parent(parent),
		m_children(new std::vector<UiObject*>()),
		m_visible(true),
		m_rectangle(new UiBound(rectangle)),
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
		delete m_rectangle;

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

		const bool aspectPosition = m_rectangle->m_flagsAspect & AspectPosition;
		const bool aspectSize = m_rectangle->m_flagsAspect & AspectSize;

		const float da = aspectSize ? Display::Get()->GetAspectRatio() : 1.0f;
		const float dw = (m_rectangle->m_dimensions->m_x / da) * m_scale;
		const float dh = m_rectangle->m_dimensions->m_y * m_scale;

		const float pa = aspectPosition ? 1.0f : Display::Get()->GetAspectRatio();
		const float px = (m_rectangle->m_position->m_x / pa) - (dw * m_rectangle->m_reference->m_x) + m_positionOffset->m_x;
		const float py = m_rectangle->m_position->m_y - (dh * (-1.0f + m_rectangle->m_reference->m_y)) + m_positionOffset->m_y;

		m_screenTransform->Set(2.0f * dw, 2.0f * dh, (2.0f * px) - 1.0f, (-2.0f * py) + 1.0f);
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
