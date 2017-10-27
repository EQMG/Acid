#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Vector2.hpp"
#include "../models/Model.hpp"
#include "../textures/Texture.hpp"
#include "../uis/UiObject.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"

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

		Vector2 *m_textureOffset;
		Colour *m_colourOffset;
	public:
		/// <summary>
		/// Creates a new GUI object.
		/// </summary>
		/// <param name="texture"> The objects texture. </param>
		/// <param name="selectedRow"> The default row of the texture to render from. </param>
		Gui(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, const Vector2 &pivot, const bool &inScreenCoords, Texture *texture, const int &selectedRow);

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

		Vector2 *GetTextureOffset() const { return m_textureOffset; }

		Colour *GetColourOffset() const { return m_colourOffset; }

		void SetColourOffset(const Colour &colourOffset) const { m_colourOffset->Set(colourOffset); }
	};
}
