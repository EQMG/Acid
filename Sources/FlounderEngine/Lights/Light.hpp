#pragma once

#include "Engine/Exports.hpp"
#include "Scenes/Objects/Component.hpp"
#include "Scenes/Objects/GameObject.hpp"
#include "Maths/Colour.hpp"
#include "Maths/Vector3.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a point light, contains a colour, position and attenuation.
	/// </summary>
	class F_EXPORT Light :
		public Component
	{
	private:
		Colour *m_colour;
		float m_radius;
		Vector3 *m_offset;
		Vector3 *m_position;

	public:
		/// <summary>
		/// Creates a new point light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="radius"> How far the light will have influence (-1 sets this to a directional light). </param>
		/// <param name="offset"> The parent offset of the light. </param>
		Light(const Colour &colour = Colour::WHITE, const float &radius = -1.0f, const Vector3 &offset = Vector3());

		/// <summary>
		/// Creates a new point light from a source object.
		/// </summary>
		/// <param name="source"> The source light object. </param>
		Light(const Light &source);

		/// <summary>
		/// Deconstructor for light.
		/// </summary>
		~Light();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		/// <summary>
		/// Sets values in the light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="offset"> The parent offset of the light. </param>
		/// <returns> This. </returns>
		Light *Set(const Colour &colour, const Vector3 &offset);

		/// <summary>
		/// Sets values in the light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="radius"> How far the light will have influence (-1 sets this to a directional light). </param>
		/// <param name="offset"> The parent offset of the light. </param>
		/// <returns> This. </returns>
		Light *Set(const Colour &colour, const float &radius, const Vector3 &offset);

		/// <summary>
		/// Sets values in the light.
		/// </summary>
		/// <param name="source"> The source light object. </param>
		/// <returns> This. </returns>
		Light *Set(const Light &source);

		std::string GetName() const override { return "Light"; };

		Colour *GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) { *m_colour = colour; }

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		Vector3 *GetOffset() const { return m_offset; }

		void SetOffset(const Vector3 &offset) { *m_offset = offset; }

		Vector3 *GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) { *m_position = position; }
	};
}
