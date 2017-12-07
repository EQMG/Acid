#pragma once

#include "../Maths/Colour.hpp"
#include "../Maths/Vector2.hpp"
#include "../Models/Model.hpp"
#include "../Textures/Texture.hpp"
#include "../Uis/UiObject.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	/// <summary>
	/// A object the represents a texture in a GUI.
	/// </summary>
	class Gui :
		public UiObject
	{
	private:
		UniformBuffer *m_uniformObject;

		Model *m_model;
		Texture *m_texture;
		int m_selectedRow;

		Vector2 *m_atlasOffset;
		Colour *m_colourOffset;
	public:
		/// <summary>
		/// Creates a new GUI object.
		/// </summary>
		/// <param name="rectangle"> The rectangle that will represent the bounds of the ui object. </param>
		/// <param name="texture"> The objects texture. </param>
		/// <param name="selectedRow"> The default row of the texture to render from. </param>
		Gui(UiObject *parent, const UiBound &rectangle, Texture *texture, const int &selectedRow);

		/// <summary>
		/// Deconstructor for the gui object.
		/// </summary>
		~Gui();

		void UpdateObject() override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline);

		Texture *GetTexture() const { return m_texture; }

		void SetTexture(Texture *texture) { m_texture = texture; }

		int GetSelectedRow() const { return m_selectedRow; }

		void SetSelectedRow(const int &selectedRow) { m_selectedRow = selectedRow; }

		Vector2 *GetAtlasOffset() const { return m_atlasOffset; }

		Colour *GetColourOffset() const { return m_colourOffset; }

		void SetColourOffset(const Colour &colourOffset) const { m_colourOffset->Set(colourOffset); }
	};
}
