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
		m_depth(0.0f),
		m_lockRotation(true),
		m_screenTransform(Vector4()),
		m_worldTransform({}),
		m_alphaDriver(std::make_unique<DriverConstant>(1.0f)),
		m_alpha(1.0f),
		m_scaleDriver(std::make_unique<DriverConstant>(1.0f)),
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
		// Click updates.
		if (IsVisible())
		{
			if (Uis::Get()->GetSelector().IsSelected(*this))
			{
				for (uint32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
				{
					if (Uis::Get()->GetSelector().WasDown(static_cast<MouseButton>(i)))
					{
						m_onClick(this, static_cast<MouseButton>(i));
					}
				}
			}

			list.emplace_back(this);
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
		float aspectRatio = m_worldTransform ? 1.0f : Display::Get()->GetAspectRatio();

		Vector2 screenDimensions = m_rectangle.GetScreenDimensions(aspectRatio);
		Vector2 screenPosition = m_rectangle.GetScreenPosition(aspectRatio);
		Vector2 referenceOffset = Vector2();

		Vector2 dimensions = screenDimensions * m_scale;
		Vector2 position = screenPosition - (dimensions * Vector2(
			m_rectangle.GetReference().m_x,
			-1.0f + m_rectangle.GetReference().m_y
		));

		if (m_parent != nullptr)
		{
		//	Vector4 parentTransform = m_parent->m_screenTransform;
		//	Vector2 parentDimensions = Vector2(parentTransform.m_x / 2.0f, parentTransform.m_y / 2.0f);
		//	Vector2 parentPosition = Vector2((parentTransform.m_z + 1.0f) / 2.0f, (parentTransform.m_w - 1.0f) / -2.0f);
		//	Log::Out("%i: %s & %s\n", list.size(), parentDimensions.ToString().c_str(), parentPosition.ToString().c_str());
		//	position -= parentPosition;
		}

		m_screenTransform = Vector4(2.0f * dimensions.m_x, 2.0f * dimensions.m_y, (2.0f * position.m_x) - 1.0f, (-2.0f * position.m_y) + 1.0f);

		// Update all children objects.
		for (auto &child : m_children)
		{
			child->Update(list);
		}
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

	bool UiObject::IsVisible() const
	{
		if (m_parent != nullptr)
		{
			return m_visible && m_parent->IsVisible();
		}

		return m_visible;
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

		return Matrix4::IDENTITY;
	}

	float UiObject::GetAlpha() const
	{
		if (m_parent != nullptr)
		{
			return m_alpha * m_parent->GetAlpha();
		}

		return m_alpha;
	}

	void UiObject::CancelEvent(const MouseButton &button) const
	{
		Uis::Get()->GetSelector().CancelWasEvent(button);
	}
}
