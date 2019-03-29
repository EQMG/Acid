#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Models/Model.hpp"
#include "Images/Image2d.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
/// <summary>
/// A object the represents a texture in a GUI.
/// </summary>
class ACID_EXPORT Gui :
	public UiObject
{
public:
	/// <summary>
	/// Creates a new GUI object.
	/// </summary>
	/// <param name="parent"> The parent screen object. </param>
	/// <param name="rectangle"> The rectangle that will represent the bounds of the ui object. </param>
	/// <param name="texture"> The objects texture. </param>
	/// <param name="colourOffset"> The texture colour offset. </param>
	Gui(UiObject *parent, const UiBound &rectangle, std::shared_ptr<Image2d> texture, const Colour &colourOffset = Colour::White);

	void UpdateObject() override;

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene);

	const std::shared_ptr<Image2d> &GetTexture() const { return m_texture; }

	void SetTexture(const std::shared_ptr<Image2d> &texture) { m_texture = texture; }

	const uint32_t &GetNumberOfRows() const { return m_numberOfRows; }

	void SetNumberOfRows(const uint32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

	const uint32_t &GetSelectedRow() const { return m_selectedRow; }

	void SetSelectedRow(const uint32_t &selectedRow) { m_selectedRow = selectedRow; }

	const Vector2 &GetAtlasOffset() const { return m_atlasOffset; }

	const Vector4 &GetNinePatces() const { return m_ninePatches; }

	/// <summary>
	/// Sets nine the patches value size, if zero it will be disabled. 9-patch/9-slicing allows for a single section
	/// of a texture to be scale with corners and edges kept in the screens aspect ratio.
	/// </summary>
	/// <param name="ninePatches"> The values, x/y being to top left corner and z/w bottom right for the scalable section. </param>
	void SetNinePatches(const Vector4 &ninePatches) { m_ninePatches = ninePatches; }

	IDriver<Colour> *GetColourDriver() const { return m_colourDriver.get(); }

	/// <summary>
	/// Sets the colour offset driver.
	/// </summary>
	/// <param name="colourDriver"> The new colour offset driver. </param>
	void SetColourDriver(IDriver<Colour> *colourDriver) { m_colourDriver.reset(colourDriver); }

	const Colour &GetColourOffset() const { return m_colourOffset; }

private:
	DescriptorsHandler m_descriptorSet;
	UniformHandler m_uniformObject;

	std::shared_ptr<Model> m_model;
	std::shared_ptr<Image2d> m_texture;
	uint32_t m_numberOfRows;
	uint32_t m_selectedRow;
	Vector2 m_atlasOffset;
	Vector4 m_ninePatches; // TODO: Use UiBound

	std::unique_ptr<IDriver<Colour>> m_colourDriver;
	Colour m_colourOffset;
};
}
