#include "UiObject.hpp"

#include "Maths/Visual/DriverConstant.hpp"
#include "Uis.hpp"

namespace acid
{
	UiObject::UiObject(UiObject *parent, const UiBound &rectangle) :
		m_parent(parent),
		m_children(std::vector<std::unique_ptr<UiObject>>()),
		m_visible(true),
		m_rectangle(UiBound(rectangle)),
		m_scissor(Vector4(0.0f, 0.0f, 1.0f, 1.0f)),
		m_positionOffset(Vector2()),
		m_screenTransform(Vector4()),
		m_worldTransform({}),
		m_alphaDriver(std::make_unique<DriverConstant>(1.0f)),
		m_alpha(1.0f),
		m_scaleDriver(std::make_unique<DriverConstant>(1.0f)),
		m_scale(1.0f),
		m_actionClick(nullptr)
	{
		if (parent != nullptr)
		{
			parent->AddChild(this);
		}
	}

	UiObject::~UiObject()
	{
		if (m_parent != nullptr)
		{
			m_parent->RemoveChild(this);
		}
	}

	void UiObject::Update(std::vector<UiObject *> &list)
	{
		// Click updates.
		if (IsVisible())
		{
			if (Uis::Get()->GetSelector().IsSelected(*this))
			{
				for (uint32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
				{
					if (Uis::Get()->GetSelector().WasDown(static_cast<MouseButton>(i)))
					{
						bool cancelWas = m_actionClick != nullptr ? m_actionClick(static_cast<MouseButton>(i)) : false;

						if (cancelWas)
						{
							Uis::Get()->GetSelector().CancelWasEvent();
							break;
						}
					}
				}
			}

			list.emplace_back(this);
		}

		for (auto &child : m_children)
		{
			child->Update(list);
		}

		// Alpha and scale updates.
		m_alpha = m_alphaDriver->Update(Engine::Get()->GetDelta());
		m_alpha = std::clamp(m_alpha, 0.0f, 1.0f);
		m_scale = m_scaleDriver->Update(Engine::Get()->GetDelta());

		if (IsVisible() && GetAlpha() != 0.0f)
		{
			UpdateObject();
		}

		// Transform updates.
		float aspectRatio = Display::Get()->GetAspectRatio();

		float da = m_rectangle.m_aspectSize ? aspectRatio : 1.0f;
		float dw = (m_rectangle.GetDimensions().m_x / da) * m_scale;
		float dh = m_rectangle.GetDimensions().m_y * m_scale;

		float pa = m_rectangle.m_aspectPosition ? 1.0f : aspectRatio;
		float px = (m_rectangle.GetPosition().m_x / pa) - (dw * m_rectangle.GetReference().m_x) + m_positionOffset.m_x;
		float py = m_rectangle.GetPosition().m_y - (dh * (-1.0f + m_rectangle.GetReference().m_y)) + m_positionOffset.m_y;

		m_screenTransform = Vector4(2.0f * dw, 2.0f * dh, (2.0f * px) - 1.0f, (-2.0f * py) + 1.0f);
	}

	void UiObject::UpdateObject()
	{
	}

	void UiObject::SetParent(UiObject *parent)
	{
		m_parent->RemoveChild(this);
		parent->AddChild(this);
		m_parent = parent;
	}

	void UiObject::AddChild(UiObject *child)
	{
		m_children.emplace_back(child);
	}

	bool UiObject::RemoveChild(UiObject *child)
	{
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			if ((*it).get() == child)
			{
				m_children.erase(it);
				return true;
			}
		}

		return false;
	}

	bool UiObject::IsVisible() const
	{
		if (m_parent != nullptr)
		{
			return m_visible && m_parent->IsVisible();
		}

		return m_visible;
	}

	float UiObject::GetAlpha() const
	{
		if (m_parent != nullptr)
		{
			return m_alpha * m_parent->GetAlpha();
		}

		return m_alpha;
	}
}
