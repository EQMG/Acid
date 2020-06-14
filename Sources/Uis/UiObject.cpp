#include "UiObject.hpp"

#include "Graphics/Graphics.hpp"
#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis.hpp"

namespace acid {
UiObject::UiObject() :
	alphaDriver(std::make_unique<ConstantDriver<float>>(1.0f)),
	scaleDriver(std::make_unique<ConstantDriver<Vector2f>>(Vector3f(1.0f))),
	screenDepth(0.0f),
	screenAlpha(1.0f),
	screenScale(1.0f) {
}

UiObject::~UiObject() {
	if (parent)
		parent->RemoveChild(this);

	for (auto &child : children)
		child->parent = nullptr;
}

void UiObject::Update(const Matrix4 &viewMatrix, std::vector<UiObject *> &list, UiObject *&cursorSelect) {
	if (!enabled) {
		return;
	}

	// Alpha and scale updates.
	alphaDriver->Update(Engine::Get()->GetDelta());
	scaleDriver->Update(Engine::Get()->GetDelta());

	UpdateObject();
	
	// Transform updates.
	constraints.Update(parent ? &parent->constraints : nullptr);

	screenPosition = {constraints.GetX()->Get(), constraints.GetY()->Get()};
	screenSize = {constraints.GetWidth()->Get(), constraints.GetHeight()->Get()};
	screenDepth = constraints.GetDepth();
	screenAlpha = alphaDriver->Get();
	screenScale = scaleDriver->Get();

	if (parent) {
		screenAlpha *= parent->screenAlpha;
		screenScale *= parent->screenScale;
	}

	auto modelMatrix = Matrix4::TransformationMatrix(Vector3f(screenPosition, 0.01f * screenDepth),
		Vector3f(), Vector3f(screenSize));
	modelView = viewMatrix * modelMatrix;

	bool selected = false;
	if (IsEnabled() && Mouse::Get()->IsWindowSelected() && Window::Get()->IsFocused()) {
		auto distance = Mouse::Get()->GetPosition() - screenPosition;
		selected = distance.x <= screenSize.x && distance.y <= screenSize.y &&
			distance.x >= 0.0f && distance.y >= 0.0f;
	}

	if (this->selected != selected) {
		this->selected = selected;
		onSelected(selected);
	}

	if (selected) {
		if (cursorHover)
			cursorSelect = this;

		for (auto button : EnumIterator<MouseButton>()) {
			if (Uis::Get()->WasDown(button))
				onClick(button);
		}
	}

	// Adds this object to the list if it is visible.
	if (screenAlpha > 0.0f)
		list.emplace_back(this);

	// Update all children objects.
	for (auto &child : children)
		child->Update(viewMatrix, list, cursorSelect);
}

void UiObject::UpdateObject() {
}

void UiObject::CancelEvent(MouseButton button) const {
	Uis::Get()->CancelWasEvent(button);
}

void UiObject::AddChild(UiObject *child) {
	if (child->parent || this == child)
		throw std::runtime_error("Adding child to UI object with an existing parent!");
	children.emplace_back(child);
	child->parent = this;
}

void UiObject::RemoveChild(UiObject *child) {
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
	child->parent = nullptr;
}

void UiObject::ClearChildren() {
	children.clear();
}

void UiObject::SetParent(UiObject *parent) {
	if (this->parent)
		this->parent->RemoveChild(this);

	if (parent)
		parent->AddChild(this);

	this->parent = parent;
}

bool UiObject::IsEnabled() const {
	// TODO: enabled getter, update enabled on object update.
	if (parent)
		return enabled && parent->IsEnabled();

	return enabled;
}
}
