#pragma once

#include <Guis/Gui.hpp>
#include <Uis/UiObject.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSinwave.hpp>

using namespace acid;

namespace test
{
class Inventory :
	public UiObject
{
public:
	explicit Inventory(UiObject* parent) :
		UiObject(parent, UiTransform(Vector2i(480, 48), Vector2i(), UiAnchor::CentreBottom))
	{
		SetScaleDriver(new DriverSinwave<Vector2f>(Vector2f(0.5f), Vector2f(1.5f), 6s));
		for (uint32_t i = 0; i < 10; i++)
		{
			auto slot = std::make_unique<Gui>(this, UiTransform(Vector2i(48, 48), Vector2i(48 * i, 0), UiAnchor::LeftTop), Image2d::Create("Guis/White.png"));
			slot->SetColourDriver(new DriverConstant<Colour>(Colour::Red.Lerp(Colour::Blue, static_cast<float>(i) / 10.0f)));
			m_slots.emplace_back(std::move(slot));
		}
	}

	void UpdateObject() override
	{
	}
private:
	std::vector<std::unique_ptr<Gui>> m_slots;
};

class Panels :
	public UiObject
{
public:
	explicit Panels(UiObject *parent);

	void UpdateObject() override;

private:
	Gui m_background;
	Inventory m_inventory;

	Gui m_guiA;
	Gui m_guiB;
	Gui m_guiC;
};
}
