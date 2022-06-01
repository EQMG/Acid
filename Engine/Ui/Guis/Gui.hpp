#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Models/Model.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Images/Image2d.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Uis/UiObject.hpp"
#include "Ui/Export.hpp"

namespace acid {
/**
 * @brief Class that represents a image UI.
 */
class ACID_UI_EXPORT Gui : public UiObject {
public:
	Gui();

	void UpdateObject() override;

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline);

	const std::shared_ptr<Image2d> &GetImage() const { return image; }
	void SetImage(const std::shared_ptr<Image2d> &image) { this->image = image; }

	uint32_t GetNumberOfRows() const { return numberOfRows; }
	void SetNumberOfRows(uint32_t numberOfRows) { this->numberOfRows = numberOfRows; }

	uint32_t GetSelectedRow() const { return selectedRow; }
	void SetSelectedRow(uint32_t selectedRow) { this->selectedRow = selectedRow; }

	const Vector2f &GetAtlasOffset() const { return atlasOffset; }

	const Vector2f &GetAtlasScale() const { return atlasScale; }
	void SetAtlasScale(const Vector2f &atlasScale) { this->atlasScale = atlasScale; }

	const Vector4f &GetNinePatches() const { return ninePatches; }
	/**
	 * Sets nine the patches value size, if zero it will be disabled. This value represents the top-left and bottom-right content bounds.
	 * 9-patch/9-slicing allows for a single section of a image to be scale with corners and edges kept in the screens aspect ratio.
	 * @param ninePatches The values, x/y being to top left corner and z/w bottom right for the scalable section.
	 */
	void SetNinePatches(const Vector4f &ninePatches) { this->ninePatches = ninePatches; }

	UiDriver<Colour> *GetColourDriver() const { return colourDriver.get(); }
	template<template<typename> typename T, typename... Args>
	auto SetColourDriver(Args &&... args) -> std::enable_if_t<std::is_convertible_v<T<Colour> *, UiDriver<Colour> *>, void> {
		colourDriver = std::make_unique<T<Colour>>(std::forward<Args>(args)...);
	}

private:
	DescriptorsHandler descriptorSet;
	UniformHandler uniformObject;

	std::unique_ptr<Model> model;
	std::shared_ptr<Image2d> image;
	uint32_t numberOfRows = 1;
	uint32_t selectedRow = 0;
	Vector2f atlasOffset;
	Vector2f atlasScale = Vector2f::One;
	Vector4f ninePatches; // TODO: Use UiTransform

	std::unique_ptr<UiDriver<Colour>> colourDriver;
};
}
