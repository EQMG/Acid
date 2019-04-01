#pragma once

#include "Scenes/Component.hpp"
#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace acid
{
/**
 * @brief Holds position, rotation, and scale components.
 */
class ACID_EXPORT Transform :
	public Component
{
public:
	/**
	 * Creates a new transform.
	 * @param position The position.
	 * @param rotation The rotation.
	 * @param scaling The scale.
	 */
	explicit Transform(const Vector3 &position = Vector3::Zero, const Vector3 &rotation = Vector3::Zero, const Vector3 &scaling = Vector3::One);

	/**
	 * Creates a new transform.
	 * @param position The position.
	 * @param rotation The rotation.
	 * @param scale The scale.
	 */
	Transform(const Vector3 &position, const Vector3 &rotation, const float &scale);

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	/**
	 * Multiplies this transform with another transform.
	 * @param other The other transform.
	 * @return The resultant transform.
	 */
	Transform Multiply(const Transform &other) const;

	Matrix4 GetWorldMatrix() const;

	const Vector3 &GetPosition() const { return m_position; }

	void SetPosition(const Vector3 &position);

	const Vector3 &GetRotation() const { return m_rotation; }

	void SetRotation(const Vector3 &rotation);

	const Vector3 &GetScaling() const { return m_scaling; }

	void SetScaling(const Vector3 &scaling);

	const bool &IsDirty() const { return m_dirty; }

	void SetDirty(const bool &dirty) const;

	bool operator==(const Transform &other) const;

	bool operator!=(const Transform &other) const;

	ACID_EXPORT friend Transform operator*(const Transform &left, const Transform &right);

	Transform &operator*=(const Transform &other);

	ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Transform &transform);

	std::string ToString() const;

	static const Transform Identity;
private:
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scaling;
	mutable Matrix4 m_worldMatrix;
	mutable bool m_dirty;
};
}
