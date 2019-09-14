#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Models/Model.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Images/Image2d.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
/**
 * @brief Class that represents a image UI.
 */
class ACID_EXPORT Gui :
	public UiObject
{
public:
	/**
	 * Creates a new GUI object.
	 * @param parent The parent screen object.
	 * @param rectangle The rectangle that will represent the bounds of the ui object.
	 * @param image The objects image.
	 * @param colourOffset The image colour offset.
	 */
	Gui(UiObject *parent, const UiTransform &rectangle, std::shared_ptr<Image2d> image, const Colour &colourOffset = Colour::White);

	void UpdateObject() override;

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline);

	const std::shared_ptr<Image2d> &GetImage() const { return m_image; }

	void SetImage(const std::shared_ptr<Image2d> &image) { m_image = image; }

	uint32_t GetNumberOfRows() const { return m_numberOfRows; }

	void SetNumberOfRows(uint32_t numberOfRows) { m_numberOfRows = numberOfRows; }

	uint32_t GetSelectedRow() const { return m_selectedRow; }

	void SetSelectedRow(uint32_t selectedRow) { m_selectedRow = selectedRow; }

	const Vector2f &GetAtlasOffset() const { return m_atlasOffset; }

	const Vector2f &GetAtlasScale() const { return m_atlasScale; }

	void SetAtlasScale(const Vector2f &atlasScale) { m_atlasScale = atlasScale; }

	const Vector4f &GetNinePatches() const { return m_ninePatches; }

	/**
	 * Sets nine the patches value size, if zero it will be disabled. This value represents the top-left and bottom-right content bounds.
	 * 9-patch/9-slicing allows for a single section of a image to be scale with corners and edges kept in the screens aspect ratio.
	 * @param ninePatches The values, x/y being to top left corner and z/w bottom right for the scalable section.
	 */
	void SetNinePatches(const Vector4f &ninePatches) { m_ninePatches = ninePatches; }

	Driver<Colour> *GetColourDriver() const { return m_colourDriver.get(); }

	/**
	 * Sets the colour offset driver.
	 * @param colourDriver The new colour offset driver.
	 */
	void SetColourDriver(std::unique_ptr<Driver<Colour>> &&colourDriver) { m_colourDriver = std::move(colourDriver); }

	const Colour &GetColourOffset() const { return m_colourOffset; }

private:
	DescriptorsHandler m_descriptorSet;
	UniformHandler m_uniformObject;

	std::shared_ptr<Model> m_model;
	std::shared_ptr<Image2d> m_image;
	uint32_t m_numberOfRows = 1;
	uint32_t m_selectedRow = 0;
	Vector2f m_atlasOffset;
	Vector2f m_atlasScale;
	Vector4f m_ninePatches; // TODO: Use UiTransform

	std::unique_ptr<Driver<Colour>> m_colourDriver;
	Colour m_colourOffset;
};
}
