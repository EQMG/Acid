#include "Text.hpp"

#include "Maths/Visual/DriverConstant.hpp"

namespace acid {
Text::Text(UiObject *parent, const UiTransform &rectangle, float fontSize, std::string text, std::shared_ptr<FontType> fontType, Justify justify,
	const Colour &textColour, float kerning, float leading) :
	UiObject(parent, rectangle),
	m_fontSize(fontSize),
	m_string(std::move(text)),
	m_justify(justify),
	m_fontType(std::move(fontType)),
	m_kerning(kerning),
	m_leading(leading),
	m_textColour(textColour),
	m_glowDriver(std::make_unique<DriverConstant<float>>(0.0f)),
	m_borderDriver(std::make_unique<DriverConstant<float>>(0.0f)) {
}

void Text::UpdateObject() {
	m_glowSize = m_glowDriver->Update(Engine::Get()->GetDelta());
	m_borderSize = m_borderDriver->Update(Engine::Get()->GetDelta());
}

void Text::SetBorderDriver(std::unique_ptr<Driver<float>> &&borderDriver) {
	m_borderDriver = std::move(borderDriver);
	m_solidBorder = true;
	m_glowBorder = false;
}

void Text::SetGlowDriver(std::unique_ptr<Driver<float>> &&glowDriver) {
	m_glowDriver = std::move(glowDriver);
	m_solidBorder = false;
	m_glowBorder = true;
}

void Text::RemoveBorder() {
	m_solidBorder = false;
	m_glowBorder = false;
}

float Text::GetTotalBorderSize() const {
	if (m_solidBorder) {
		if (m_borderSize == 0.0f)
			return 0.0f;

		return CalculateEdgeStart() + m_borderSize;
	}

	if (m_glowBorder)
		return CalculateEdgeStart();
	return 0.0f;
}

float Text::GetGlowSize() const {
	if (m_solidBorder)
		return CalculateAntialiasSize();
	if (m_glowBorder)
		return m_glowSize;
	return 0.0f;
}

float Text::CalculateEdgeStart() const {
	auto scale = GetScale() * m_fontSize;
	auto size = 0.5f * scale.m_x;
	return 1.0f / 300.0f * size + 137.0f / 300.0f;
}

float Text::CalculateAntialiasSize() const {
	auto scale = GetScale() * m_fontSize;
	auto size = 0.5f * scale.m_x;
	size = (size - 1.0f) / (1.0f + size / 4.0f) + 1.0f;
	return 0.1f / size;
}
}
