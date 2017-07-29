#pragma once

#include <functional>

#include "../devices/display.h"
#include "../maths/vector2.h"
#include "../maths/vector4.h"
#include "../maths/colour.h"
#include "../textures/texture.h"

namespace flounder
{
	class entity
	{
	private:
		std::vector<std::function<void(entity *)>> *m_components;

		bool m_visible;
		vector2 *m_position;
		vector2 *m_dimensions;
		vector2 *m_meshSize;

		vector2 *m_screenPosition;
		vector2 *m_screenDimensions;
		vector2 *m_positionOffsets;

		texture *m_texture;
		bool m_flipTexture;
		int m_selectedRow;

		vector2 *m_textureOffset;
		colour *m_colourOffset;

		float m_rotation;
		float m_alpha;
		float m_scale;
	public:
		entity(const vector2 &position, const vector2 &dimensions, texture *texture, const int &selectedRow);

		~entity();

		void update();

		void addComponent(std::function<void(entity *)> component);

		bool isVisible() const { return m_visible; }

		inline void setVisible(const bool &visible) { m_visible = visible; }

		inline vector2 *getPosition() const { return m_position; }

		inline void setPosition(const vector2 &position) const { m_position->set(position); }

		inline vector2 *getDimensions() const { return m_dimensions; }

		inline void setDimensions(const vector2 &dimensions) const { m_dimensions->set(dimensions); }

		inline vector2 *getMeshSize() const { return m_meshSize; }

		inline void setMeshSize(const vector2 &meshSize) const { m_meshSize->set(meshSize); }

		/// <summary>
		/// Gets the positions relative in screen space.
		/// </summary>
		/// <returns> The screen positions. </returns>
		inline vector2 *getScreenPosition() const { return m_screenPosition; }

		/// <summary>
		/// Gets the dimensions relative in screen space.
		/// </summary>
		/// <returns> The screen dimensions. </returns>
		inline vector2 *getScreenDimensions() const { return m_screenDimensions; }

		inline vector2 *getPositionOffsets() const { return m_positionOffsets; }

		inline void setPositionOffsets(const vector2 &positionOffsets) const { m_positionOffsets->set(positionOffsets); }

		texture *getTexture() const { return m_texture; }

		void setTexture(texture *texture) { m_texture = texture; }

		bool getFlipTexture() const { return m_flipTexture; }

		void setFlipTexture(const bool &flipTexture) { m_flipTexture = flipTexture; }

		int getSelectedRow() const { return m_selectedRow; }

		void setSelectedRow(const int &selectedRow) { m_selectedRow = selectedRow; }

		vector2 *getTextureOffset() const { return m_textureOffset; }

		colour *getColourOffset() const { return m_colourOffset; }

		void setColourOffset(const colour &colourOffset) const { m_colourOffset->set(colourOffset); }

		inline float getRotation() const { return m_rotation; }

		void setRotation(float rotation)
		{
			m_rotation = rotation;
		}

		float getAlpha() const { return m_alpha; }

		void setAlpha(float alpha)
		{
			m_alpha = alpha;
		}

		inline float getScale() const { return m_scale; }

		void setScale(float scale)
		{
			m_scale = scale;
		}
	};
}
