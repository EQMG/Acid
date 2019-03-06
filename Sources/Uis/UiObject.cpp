#include "UiObject.hpp"

#include "Renderer/Renderer.hpp"
#include "Maths/Visual/DriverConstant.hpp"
#include "Uis.hpp"

namespace acid
{
	UiObject::UiObject(UiObject *parent, const UiBound &rectangle) :
		m_parent(parent),
		m_enabled(true),
		m_rectangle(rectangle),
		m_scissor(0.0f, 0.0f, 1.0f, 1.0f),
		m_height(0.0f),
		m_lockRotation(true),
		m_worldTransform({}),
		m_alphaDriver(std::make_unique<DriverConstant<float>>(1.0f)),
		m_alpha(1.0f),
		m_scaleDriver(std::make_unique<DriverConstant<float>>(1.0f)),
		m_scale(1.0f),
		m_screenDepth(0.0f),
		m_screenAlpha(1.0f),
		m_screenScale(1.0f)
	{
		if (m_parent != nullptr)
		{
			m_parent->AddChild(this);
		}
	}

	UiObject::~UiObject()
	{
		if (m_parent != nullptr)
		{
			m_parent->RemoveChild(this);
		}

		for (auto &child : m_children)
		{
			child->m_parent = nullptr;
		}
	}

	void UiObject::Update(std::vector<UiObject *> &list)
	{
		if (!m_enabled)
		{
			return;
		}

		if (IsSelected())
		{
			for (auto button : enum_iterator<MouseButton>())
			{
				if (Uis::Get()->WasDown(button))
				{
					m_onClick(this, button);
				}
			}
		}

		// Alpha and scale updates.
		m_alpha = m_alphaDriver->Update(Engine::Get()->GetDelta());
		m_scale = m_scaleDriver->Update(Engine::Get()->GetDelta());

		UpdateObject();

		// Transform updates.
		float aspectRatio = m_worldTransform ? 1.0f : Window::Get()->GetAspectRatio();

		m_screenDimensions = m_rectangle.GetScreenDimensions(aspectRatio) * m_scale;
		m_screenDepth = 0.01f * m_height;
		m_screenScale = m_scale;
		m_screenAlpha = m_alpha;

		if (m_parent != nullptr)
		{
			if (m_rectangle.GetAspect() & UiAspect::Scale)
			{
				m_screenDimensions *= m_parent->m_screenDimensions;
				m_screenScale *= m_parent->m_screenScale;
			}

			m_screenPosition = (m_rectangle.GetScreenPosition(aspectRatio) * m_parent->m_screenDimensions) - (m_screenDimensions * m_rectangle.GetReference()) + m_parent->m_screenPosition;
			m_screenAlpha *= m_parent->m_screenAlpha;
		}
		else
		{
			m_screenPosition = m_rectangle.GetScreenPosition(aspectRatio) - (m_screenDimensions * m_rectangle.GetReference());
		}

		// Adds this object to the list if it is visible.
		if (m_screenAlpha > 0.0f)
		{
			list.emplace_back(this);
		}

		// Update all children objects.
		for (auto &child : m_children)
		{
			child->Update(list);
		}
	}

	void UiObject::UpdateObject()
	{
	}

	bool UiObject::IsSelected() const
	{
		if (!m_enabled)
		{
			return false;
		}

		if (Mouse::Get()->IsWindowSelected() && Window::Get()->IsFocused())
		{
			Vector2 distance = Mouse::Get()->GetPosition() - m_screenPosition;
			return distance.m_x >= 0.0f && distance.m_y >= 0.0f && distance.m_x <= m_screenDimensions.m_x && distance.m_y <= m_screenDimensions.m_y;
		}

		return false;
	}

	void UiObject::SetParent(UiObject *parent)
	{
		if (m_parent != nullptr)
		{
			m_parent->RemoveChild(this);
		}

		if (parent != nullptr)
		{
			parent->AddChild(this);
		}

		m_parent = parent;
	}

	void UiObject::AddChild(UiObject *child)
	{
		m_children.emplace_back(child);
	}

	void UiObject::RemoveChild(UiObject *child)
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
	}

	bool UiObject::IsEnabled() const
	{
		// TODO: m_enabled getter, update m_enabled on object update.
		if (m_parent != nullptr)
		{
			return m_enabled && m_parent->IsEnabled();
		}

		return m_enabled;
	}

	Matrix4 UiObject::GetModelMatrix() const
	{
		if (m_worldTransform)
		{
			Matrix4 worldMatrix = m_worldTransform->GetWorldMatrix();

			if (m_lockRotation)
			{
				Vector3 scaling = m_worldTransform->GetScaling();

				for (uint32_t i = 0; i < 3; i++)
				{
					worldMatrix[0][i] = scaling[i];
				}
			}

			return worldMatrix;
		}

		if (m_parent != nullptr)
		{
			return m_parent->GetModelMatrix(); // TODO: Multiply by this 'local' WorldMatrix.
		}

		return Matrix4::Identity;
	}

	void UiObject::CancelEvent(const MouseButton &button) const
	{
		Uis::Get()->CancelWasEvent(button);
	}
}
