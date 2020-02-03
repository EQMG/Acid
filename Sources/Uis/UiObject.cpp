#include "UiObject.hpp"

#include "Graphics/Graphics.hpp"
#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis.hpp"

namespace acid {
UiObject::UiObject() :
	m_alphaDriver(std::make_unique<ConstantDriver<float>>(1.0f)),
	m_scaleDriver(std::make_unique<ConstantDriver<Vector2f>>(Vector3f(1.0f))),
	m_screenDepth(0.0f),
	m_screenAlpha(1.0f),
	m_screenScale(1.0f) {
}

UiObject::~UiObject() {
	if (m_parent)
		m_parent->RemoveChild(this);

	for (auto &child : m_children)
		child->m_parent = nullptr;
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
	m_constraints.Update(m_parent ? &m_parent->m_constraints : nullptr);

	m_screenPosition = {m_constraints.GetX()->Get(), m_constraints.GetY()->Get()};
	m_screenSize = {m_constraints.GetWidth()->Get(), m_constraints.GetHeight()->Get()};
	m_screenDepth = m_constraints.GetDepth();
	m_screenAlpha = m_alphaDriver->Get();
	m_screenScale = m_scaleDriver->Get();

	if (m_parent) {
		m_screenAlpha *= m_parent->m_screenAlpha;
		m_screenScale *= m_parent->m_screenScale;
	}

	auto modelMatrix = Matrix4::TransformationMatrix(Vector3f(m_screenPosition, 0.01f * m_screenDepth),
		Vector3f(), Vector3f(m_screenSize));
	m_modelView = viewMatrix * modelMatrix;

	bool selected = false;
	if (IsEnabled() && Mouse::Get()->IsWindowSelected() && Window::Get()->IsFocused()) {
		auto distance = Mouse::Get()->GetPosition() - m_screenPosition;
		selected = distance.m_x <= m_screenSize.m_x && distance.m_y <= m_screenSize.m_y &&
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

void UiObject::AddChild(UiObject *child) {
	if (child->m_parent || this == child) {
		throw std::runtime_error("Adding child to UI object with an existing parent!");
	}
	m_children.emplace_back(child);
	child->m_parent = this;
}

void UiObject::RemoveChild(UiObject *child) {
	m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
	child->m_parent = nullptr;
}

void UiObject::ClearChildren() {
	m_children.clear();
}

void UiObject::SetParent(UiObject *parent) {
	if (m_parent)
		m_parent->RemoveChild(this);

	if (parent)
		parent->AddChild(this);

	m_parent = parent;
}

bool UiObject::IsEnabled() const {
	// TODO: m_enabled getter, update m_enabled on object update.
	if (m_parent) {
		return m_enabled && m_parent->IsEnabled();
	}

	return m_enabled;
}
}
