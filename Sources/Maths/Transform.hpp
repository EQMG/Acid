#pragma once

#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "Scenes/Component.hpp"

namespace acid {
/**
 * @brief Holds position, rotation, and scale components.
 */
class ACID_EXPORT Transform : public Component::Registrar<Transform> {
	inline static const bool Registered = Register("transform");
public:
	/**
	 * Creates a new transform.
	 * @param position The position.
	 * @param rotation The rotation.
	 * @param scale The scale.
	 */
	Transform(const Vector3f &position = {}, const Vector3f &rotation = {}, const Vector3f &scale = Vector3f(1.0f));
	~Transform();

	/**
	 * Multiplies this transform with another transform.
	 * @param other The other transform.
	 * @return The resultant transform.
	 */
	Transform Multiply(const Transform &other) const;

	Matrix4 GetWorldMatrix() const;
	Vector3f GetPosition() const;
	Vector3f GetRotation() const;
	Vector3f GetScale() const;

	const Vector3f &GetLocalPosition() const { return position; }
	void SetLocalPosition(const Vector3f &localPosition) { position = localPosition; }

	const Vector3f &GetLocalRotation() const { return rotation; }
	void SetLocalRotation(const Vector3f &localRotation) { rotation = localRotation; }

	const Vector3f &GetLocalScale() const { return scale; }
	void SetLocalScale(const Vector3f &localScale) { scale = localScale; }

	Transform *GetParent() const { return parent; }
	void SetParent(Transform *parent);
	void SetParent(Entity *parent);

	const std::vector<Transform *> &GetChildren() const { return children; }

	bool operator==(const Transform &rhs) const;
	bool operator!=(const Transform &rhs) const;

	friend Transform operator*(const Transform &lhs, const Transform &rhs);

	Transform &operator*=(const Transform &rhs);

	friend const Node &operator>>(const Node &node, Transform &transform);
	friend Node &operator<<(Node &node, const Transform &transform);
	friend std::ostream &operator<<(std::ostream &stream, const Transform &transform);

private:
	const Transform *GetWorldTransform() const;

	void AddChild(Transform *child);
	void RemoveChild(Transform *child);

	Vector3f position;
	Vector3f rotation;
	Vector3f scale;

	Transform *parent = nullptr;
	std::vector<Transform *> children;
	mutable Transform *worldTransform = nullptr;
};
}
