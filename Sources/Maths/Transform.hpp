#pragma once

#include "Engine/Exports.hpp"
#include "Scenes/Component.hpp"
#include "Matrix4.hpp"
#include "Vector3.hpp"

namespace acid
{
	class Packet;
	class Metadata;

	/// <summary>
	/// Holds position, rotation, and scale components.
	/// </summary>
	class ACID_EXPORT Transform :
		public Component
	{
	private:
		Vector3 m_position;
		Vector3 m_rotation;
		Vector3 m_scaling;
		mutable Matrix4 m_worldMatrix;
		mutable bool m_dirty;
	public:
		static const Transform IDENTITY;

		/// <summary>
		/// Constructor for Transform.
		/// </summary>
		/// <param name="position"> The position. </param>
		/// <param name="rotation"> The rotation. </param>
		/// <param name="scaling"> The scaling. </param>
		explicit Transform(const Vector3 &position = Vector3::ZERO, const Vector3 &rotation = Vector3::ZERO, const Vector3 &scaling = Vector3::ONE);

		/// <summary>
		/// Constructor for Transform.
		/// </summary>
		/// <param name="position"> The position. </param>
		/// <param name="rotation"> The rotation. </param>
		/// <param name="scale"> The scale. </param>
		Transform(const Vector3 &position, const Vector3 &rotation, const float &scale);

		/// <summary>
		/// Constructor for Transform.
		/// </summary>
		/// <param name="source"> Creates this vector out of a transform. </param>
		Transform(const Transform &source);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		/// <summary>
		/// Multiplies this transform with another transform.
		/// </summary>
		/// <param name="other"> The other transform. </param>
		/// <returns> The resultant transform. </returns>
		Transform Multiply(const Transform &other) const;

		Matrix4 GetWorldMatrix() const;

		Vector3 GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position);

		Vector3 GetRotation() const { return m_rotation; }

		void SetRotation(const Vector3 &rotation);

		Vector3 GetScaling() const { return m_scaling; }

		void SetScaling(const Vector3 &scaling);

		bool IsDirty() const { return m_dirty; }

		void SetDirty(const bool &dirty) const;

		bool operator==(const Transform &other) const;

		bool operator!=(const Transform &other) const;

		ACID_EXPORT friend Transform operator*(const Transform &left, const Transform &right);

		Transform &operator*=(const Transform &other);

		ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Transform &transform);

		ACID_EXPORT friend Packet &operator<<(Packet &packet, const Transform &transform);

		ACID_EXPORT friend Packet &operator>>(Packet &packet, Transform &transform);

		std::string ToString() const;
	};
}
