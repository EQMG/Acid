#include "UiObject.hpp"

#include "Graphics/Graphics.hpp"
#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis.hpp"

namespace acid {
UiObject::UiObject(UiObject *parent, const UiTransform &transform) :
	m_parent(parent),
	m_enabled(true),
	m_transform(transform),
	m_alphaDriver(std::make_unique<ConstantDriver<float>>(1.0f)),
	m_scaleDriver(std::make_unique<ConstantDriver<Vector2f>>(Vector3f(1.0f))),
	m_screenAlpha(1.0f),
	m_screenScale(1.0f) {
	if (m_parent) {
		m_parent->AddChild(this);
	}
}

UiObject::~UiObject() {
	if (m_parent) {
		m_parent->RemoveChild(this);
	}

	for (auto &child : m_children) {
		child->m_parent = nullptr;
	}
}

void UiObject::Update(const Matrix4 &viewMatrix, std::vector<UiObject *> &list, UiObject *&cursorSelect) {
	if (!m_enabled) {
		return;
	}

	// Alpha and scale updates.
	m_alphaDriver->Update(Engine::Get()->GetDelta());
	m_scaleDriver->Update(Engine::Get()->GetDelta());

	UpdateObject();

	// Transform updates.
	m_screenAlpha = m_alphaDriver->Get();
	m_screenScale = m_scaleDriver->Get();
	m_screenTransform = m_transform;
	m_screenTransform.SetAnchor(UiAnchor::LeftTop);

	if (m_parent) {
		m_screenAlpha *= m_parent->m_screenAlpha;
		m_screenScale *= m_parent->m_screenScale;

		m_screenTransform.m_position *= m_screenScale;
	}

	m_screenTransform.m_size *= m_screenScale;

	if (m_transform.m_margins) {
		if (m_parent) {
			Vector2f leftTop(m_transform.GetAnchor0());

			if (*m_transform.m_margins & UiMargins::Left) {
				leftTop.m_x = m_parent->m_screenTransform.m_size.m_x * leftTop.m_x;
			}

			if (*m_transform.m_margins & UiMargins::Top) {
				leftTop.m_y = m_parent->m_screenTransform.m_size.m_y * leftTop.m_y;
			}

			Vector2f rightBottom;

			if (*m_transform.m_margins & UiMargins::Right) {
				rightBottom.m_x = m_parent->m_screenTransform.m_size.m_x * (1.0f + m_transform.GetAnchor1().m_x);
			} else {
				rightBottom.m_x = m_parent->m_screenTransform.m_size.m_x + m_transform.GetAnchor1().m_x;
			}

			if (*m_transform.m_margins & UiMargins::Bottom) {
				rightBottom.m_y = m_parent->m_screenTransform.m_size.m_y * (1.0f + m_transform.GetAnchor1().m_y);
			} else {
				rightBottom.m_y = m_parent->m_screenTransform.m_size.m_y + m_transform.GetAnchor1().m_y;
			}

			m_screenTransform.m_size = rightBottom - leftTop;
			m_screenTransform.m_position = leftTop + m_parent->m_screenTransform.m_position;
		} else {
			/*if (*m_transform.m_margins != UiMargins::None) {
				throw std::runtime_error("UiTransform with a percent margin must have a parent");
			}*/

			m_screenTransform.m_size = m_transform.GetAnchor1() - m_transform.GetAnchor0();
			m_screenTransform.m_position = m_transform.GetAnchor0();
		}
	} else {
		if (m_parent) {
			m_screenTransform.m_position += m_parent->m_screenTransform.m_size * m_transform.GetAnchor0();
			m_screenTransform.m_position += m_parent->m_screenTransform.m_position;
		}

		m_screenTransform.m_position -= m_screenTransform.m_size * m_transform.GetAnchor1();
	}

	auto modelMatrix = Matrix4::TransformationMatrix(Vector3f(m_screenTransform.m_position, 0.01f * m_screenTransform.m_depth), 
		Vector3f(), Vector3f(m_screenTransform.m_size));
	m_modelView = viewMatrix * modelMatrix;

	bool selected = false;

	if (IsEnabled() && Mouse::Get()->IsWindowSelected() && Window::Get()->IsFocused()) {
		auto distance = Mouse::Get()->GetPosition() - m_screenTransform.m_position;
		selected = distance.m_x <= m_screenTransform.m_size.m_x && distance.m_y <= m_screenTransform.m_size.m_y &&
			distance.m_x >= 0.0f && distance.m_y >= 0.0f;
	}

	if (selected != m_selected) {
		m_selected = selected;
		m_onSelected(m_selected);
	}

	if (m_selected) {
		if (m_cursorHover) {
			cursorSelect = this;
		}

		for (auto button : EnumIterator<MouseButton>()) {
			if (Uis::Get()->WasDown(button)) {
				m_onClick(button);
			}
		}
	}

	// Adds this object to the list if it is visible.
	if (m_screenAlpha > 0.0f) {
		list.emplace_back(this);
	}

	// Update all children objects.
	for (auto &child : m_children) {
		child->Update(viewMatrix, list, cursorSelect);
	}
}

void UiObject::UpdateObject() {
}

void UiObject::CancelEvent(MouseButton button) const {
	Uis::Get()->CancelWasEvent(button);
}

void UiObject::SetParent(UiObject *parent) {
	if (m_parent) {
		m_parent->RemoveChild(this);
	}

	if (parent) {
		parent->AddChild(this);
	}

	m_parent = parent;
}

void UiObject::AddChild(UiObject *child) {
	m_children.emplace_back(child);
}

void UiObject::RemoveChild(UiObject *child) {
	m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}

bool UiObject::IsEnabled() const {
	// TODO: m_enabled getter, update m_enabled on object update.
	if (m_parent) {
		return m_enabled && m_parent->IsEnabled();
	}

	return m_enabled;
}
}
