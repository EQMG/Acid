#pragma once

#include "../Maths/Transform.hpp"
#include "../Objects/Component.hpp"
#include "../Objects/GameObject.hpp"
#include "Frustum.hpp"
#include "Intersect.hpp"
#include "Ray.hpp"

namespace Flounder
{
	/// <summary>
	/// A simple class that represents a physical shape.
	/// </summary>
	class F_EXPORT Collider :
		public Component
	{
	public:
		/// <summary>
		/// Creates a new collider.
		/// </summary>
		Collider();

		/// <summary>
		/// Deconstructor for the collider.
		/// </summary>
		~Collider();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "Collider"; };

		/// <summary>
		/// Clones this collder into the destination and updates it.
		/// </summary>
		/// <param name="transform"> The amount to transform the object. </param>
		/// <param name="destination"> The collider to store the new data in. </param>
		/// <returns> The destination. </returns>
		virtual Collider *Update(const Transform &transform, Collider *destination) = 0;

		/// <summary>
		/// Adjusts a movement amount so that after the move is performed, the this collider will not intersect the {@code right}.
		/// This method assumes that this collider can actually intersect {@code right} after some amount of movement,
		/// even if it won't necessarily intersect it after the movement specified by {@code moveDelta}.
		/// </summary>
		/// <param name="other"> The right source collider. </param>
		/// <param name="positionDelta"> The delta movement for the left collider. </param>
		/// <param name="destination"> Where the final resolved delta should be stored. </param>
		/// <returns> The new, adjusted verifyMove delta that guarantees no intersection. </returns>
		virtual Vector3 *ResolveCollision(const Collider &other, const Vector3 &positionDelta, Vector3 *destination) = 0;

		/// <summary>
		/// Tests whether a shape is intersecting this shape.
		/// </summary>
		/// <param name="other"> The other shape being tested for intersection. </param>
		/// <returns> Data about the calculated shape intersection. </returns>
		virtual Intersect Intersects(const Collider &other) = 0;

		/// <summary>
		/// Tests whether a ray is intersecting this shape.
		/// </summary>
		/// <param name="ray"> The ray being tested for intersection. </param>
		/// <returns> Data about the calculated ray intersection. </returns>
		virtual Intersect Intersects(const Ray &ray) = 0;

		/// <summary>
		/// Gets if the shape is partially in the view frustum.
		/// </summary>
		/// <param name="frustum"> The view frustum. </param>
		/// <returns> If the shape is partially in the view frustum. </returns>
		virtual bool InFrustum(const Frustum &frustum) = 0;

		/// <summary>
		/// Tests whether another this shape completely contains the other.
		/// </summary>
		/// <param name="other"> The shape being tested for containment. </param>
		/// <returns> True if {@code other} is contained by this shape, false otherwise. </returns>
		virtual bool Contains(const Collider &other) = 0;

		/// <summary>
		/// Gets if a point is contained in this shape.
		/// </summary>
		/// <param name="point"> The point to check if it is contained. </param>
		/// <returns> If the point is contained in this shape. </returns>
		virtual bool Contains(const Vector3 &point) = 0;

		/*/// <summary>
		/// Gets the (optional) model to be used in the <seealso cref="BoundingRenderer"/>.
		/// </summary>
		/// <returns> A model that can be used to render this shape. </returns>
		virtual Model *GetRenderModel() = 0;

		/// <summary>
		/// Gets the centre for the rendered model.
		/// </summary>
		/// <param name="destination"> The destination for the information. </param>
		/// <returns> The centre for the rendered model. </returns>
		virtual Vector3 *GetRenderCentre(Vector3 *destination) = 0;

		/// <summary>
		/// Gets the rotation for the rendered model.
		/// </summary>
		/// <param name="destination"> The destination for the information. </param>
		/// <returns> The rotation for the rendered model. </returns>
		virtual Vector3 *GetRenderRotation(Vector3 *destination) = 0;

		/// <summary>
		/// Gets the scale for the rendered model.
		/// </summary>
		/// <param name="destination"> The destination for the information. </param>
		/// <returns> The scale for the rendered model. </returns>
		virtual Vector3 *GetRenderScale(Vector3 *destination) = 0;

		/// <summary>
		/// Gets the colour for the rendered model.
		/// </summary>
		/// <param name="destination"> The destination for the information. </param>
		/// <returns> The colour for the rendered model. </returns>
		virtual Colour *GetRenderColour(Colour *destination) = 0;*/
	};
}
