#pragma once

#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace acid
{
/**
 * @brief Holds position, rotation, and scale components.
 */
class ACID_EXPORT Transform
{
public:
	/**
	 * Creates a new transform.
	 * @param position The position.
	 * @param rotation The rotation.
	 * @param scaling The scale.
	 */
	explicit Transform(const Vector3f &position = Vector3f::Zero, const Vector3f &rotation = Vector3f::Zero, const Vector3f &scaling = Vector3f::One);

	/**
	 * Creates a new transform.
	 * @param position The position.
	 * @param rotation The rotation.
	 * @param scale The scale.
	 */
	Transform(const Vector3f &position, const Vector3f &rotation, const float &scale);

	void Decode(const Metadata &metadata);

	void Encode(Metadata &metadata) const;

	/**
	 * Multiplies this transform with another transform.
	 * @param other The other transform.
	 * @return The resultant transform.
	 */
	Transform Multiply(const Transform &other) const;

	Matrix4 GetWorldMatrix() const;

	const Vector3f &GetPosition() const { return m_position; }

	void SetPosition(const Vector3f &position);

	const Vector3f &GetRotation() const { return m_rotation; }

	void SetRotation(const Vector3f &rotation);

	const Vector3f &GetScaling() const { return m_scaling; }

	void SetScaling(const Vector3f &scaling);

	const bool &IsDirty() const { return m_dirty; }

	void SetDirty(const bool &dirty) const;

	bool operator==(const Transform &other) const;

	bool operator!=(const Transform &other) const;

	ACID_EXPORT friend Transform operator*(const Transform &left, const Transform &right);

	Transform &operator*=(const Transform &other);

	ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Transform &transform);

	std::string ToString() const;

	static const Transform Zero;
private:
	Vector3f m_position;
	Vector3f m_rotation;
	Vector3f m_scaling;
	mutable Matrix4 m_worldMatrix;
	mutable bool m_dirty;
};
}
