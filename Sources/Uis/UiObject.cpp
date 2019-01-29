#include "UiObject.hpp"

#include "Renderer/Renderer.hpp"
#include "Maths/Visual/DriverConstant.hpp"
#include "Uis.hpp"

namespace acid
{
	UiObject::UiObject(UiObject *parent, const UiBound &rectangle) :
		m_parent(parent),
		m_children(std::vector<std::unique_ptr<UiObject>>()),
		m_enabled(true),
		m_rectangle(UiBound(rectangle)),
		m_scissor(Vector4(0.0f, 0.0f, 1.0f, 1.0f)),
		m_height(0.0f),
		m_screenDimension(Vector2()),
		m_screenPosition(Vector2()),
		m_screenDepth(0.0f),
		m_lockRotation(true),
		m_worldTransform({}),
		m_alphaDriver(std::make_unique<DriverConstant<float>>(1.0f)),
		m_alpha(1.0f),
		m_scaleDriver(std::make_unique<DriverConstant<float>>(1.0f)),
		m_scale(1.0f),
		m_onClick(Delegate<void(UiObject *, MouseButton)>())
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

		if (GetAlpha() > 0.0f)
		{
			UpdateObject();
			list.emplace_back(this);
		}

		// Alpha and scale updates.
		m_alpha = m_alphaDriver->Update(Engine::Get()->GetDelta());
		m_scale = m_scaleDriver->Update(Engine::Get()->GetDelta());

		// Transform updates.
		float aspectRatio = m_worldTransform ? 1.0f : Window::Get()->GetAspectRatio();

		m_screenDimension = m_rectangle.GetScreenDimensions(aspectRatio) * m_scale;

		if (m_parent != nullptr)
		{
			if (m_rectangle.GetAspect() & UiAspect::Scale)
			{
				m_screenDimension *= m_parent->m_screenDimension;
			}

			m_screenPosition = (m_rectangle.GetScreenPosition(aspectRatio) * m_parent->m_screenDimension) - (m_screenDimension * m_rectangle.GetReference()) + m_parent->m_screenPosition;
		}
		else
		{
			m_screenPosition = m_rectangle.GetScreenPosition(aspectRatio) - (m_screenDimension * m_rectangle.GetReference());
		}

		m_screenDepth = 0.01f * m_height;

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
			return distance >= Vector2::Zero && distance <= m_screenDimension;
		}

		return false;
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

	void UiObject::RemoveChild(UiObject *child)
	{
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			if ((*it).get() == child)
			{
				m_children.erase(it);
			}
		}
	}

	bool UiObject::IsEnabled() const
	{
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

	float UiObject::GetAlpha() const
	{
		if (m_parent != nullptr)
		{
			return m_alpha * m_parent->GetAlpha();
		}

		return m_alpha;
	}

	float UiObject::GetScale() const
	{
		if (m_parent != nullptr && m_rectangle.GetAspect() & UiAspect::Scale)
		{
			return m_scale * m_parent->GetScale();
		}

		return m_scale;
	}

	void UiObject::CancelEvent(const MouseButton &button) const
	{
		Uis::Get()->CancelWasEvent(button);
	}
}
