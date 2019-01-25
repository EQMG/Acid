#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Models/Model.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Textures/Texture.hpp"
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
		/// <param name="rectangle"> The rectangle that will represent the bounds of the ui object. </param>
		/// <param name="texture"> The objects texture. </param>
		/// <param name="colourOffset"> The texture colour offset. </param>
		Gui(UiObject *parent, const UiBound &rectangle, const std::shared_ptr<Texture> &texture, const Colour &colourOffset = Colour::White);

		void UpdateObject() override;

		bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene);

		const std::shared_ptr<Texture> &GetTexture() const { return m_texture; }

		void SetTexture(const std::shared_ptr<Texture> &texture) { m_texture = texture; }

		const uint32_t &GetNumberOfRows() const { return m_numberOfRows; }

		void SetNumberOfRows(const uint32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

		const uint32_t &GetSelectedRow() const { return m_selectedRow; }

		void SetSelectedRow(const uint32_t &selectedRow) { m_selectedRow = selectedRow; }

		const Vector2 &GetAtlasOffset() const { return m_atlasOffset; }

		const Colour &GetColourOffset() const { return m_colourOffset; }

		void SetColourOffset(const Colour &colourOffset) { m_colourOffset = colourOffset; }
	private:
		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformObject;

		std::shared_ptr<Model> m_model;
		std::shared_ptr<Texture> m_texture;
		Colour m_colourOffset;
		uint32_t m_numberOfRows;
		uint32_t m_selectedRow;

		Vector2 m_atlasOffset;
	};
}
