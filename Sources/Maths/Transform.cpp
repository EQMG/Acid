#include "Transform.hpp"

#include "Network/Packet.hpp"
#include "Scenes/Entity.hpp"
#include "Serialized/Metadata.hpp"
#include "Quaternion.hpp"

namespace acid
{
	const Transform Transform::IDENTITY = Transform();

	Transform::Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scaling) :
		m_position(position),
		m_rotation(rotation),
		m_scaling(scaling),
		m_worldMatrix(Matrix4()),
		m_dirty(true)
	{
	}

	Transform::Transform(const Vector3 &position, const Vector3 &rotation, const float &scale) :
		m_position(position),
		m_rotation(rotation),
		m_scaling(Vector3(scale, scale, scale)),
		m_worldMatrix(Matrix4()),
		m_dirty(true)
	{
	}

	Transform::Transform(const Transform &source) :
		m_position(source.m_position),
		m_rotation(source.m_rotation),
		m_scaling(source.m_scaling),
		m_worldMatrix(Matrix4()),
		m_dirty(true)
	{
	}

	void Transform::Start()
	{
	}

	void Transform::Update()
	{
	}

	void Transform::Decode(const Metadata &metadata)
	{
		m_position = metadata.GetChild<Vector3>("Position");
		m_rotation = metadata.GetChild<Vector3>("Rotation");
		m_scaling = metadata.GetChild<Vector3>("Scaling");
		m_dirty = true;
	}

	void Transform::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Vector3>("Position", m_position);
		metadata.SetChild<Vector3>("Rotation", m_rotation);
		metadata.SetChild<Vector3>("Scaling", m_scaling);
	}

	Transform Transform::Multiply(const Transform &other) const
	{
		return Transform(GetWorldMatrix().Transform(other.m_position), m_rotation + other.m_rotation, m_scaling * other.m_scaling);
	}

	Matrix4 Transform::GetWorldMatrix() const
	{
		if (m_dirty)
		{
			m_worldMatrix = Matrix4::TransformationMatrix(m_position, m_rotation, m_scaling);
			m_dirty = false;
		}

		return m_worldMatrix;
	}

	void Transform::SetPosition(const Vector3 &position)
	{
		if (m_position != position)
		{
			m_position = position;
			m_dirty = true;
		}
	}

	void Transform::SetRotation(const Vector3 &rotation)
	{
		if (m_rotation != rotation)
		{
			m_rotation = rotation;
			m_dirty = true;
		}
	}

	void Transform::SetScaling(const Vector3 &scaling)
	{
		if (m_scaling != scaling)
		{
			m_scaling = scaling;
			m_dirty = true;
		}
	}

	void Transform::SetDirty(const bool &dirty) const
	{
		m_dirty = dirty;
		m_worldMatrix = Matrix4::TransformationMatrix(m_position, m_rotation, m_scaling);
	}

	bool Transform::operator==(const Transform &other) const
	{
		return m_position == other.m_position && m_rotation == other.m_rotation && m_scaling == other.m_scaling;
	}

	bool Transform::operator!=(const Transform &other) const
	{
		return !(*this == other);
	}

	Transform operator*(const Transform &left, const Transform &right)
	{
		return left.Multiply(right);
	}

	Transform &Transform::operator*=(const Transform &other)
	{
		return *this = Multiply(other);
	}

	std::ostream &operator<<(std::ostream &stream, const Transform &transform)
	{
		stream << transform.ToString();
		return stream;
	}

	Packet &operator<<(Packet &packet, const Transform &transform)
	{
		return packet << transform.m_position << transform.m_rotation << transform.m_scaling;
	}

	Packet &operator>>(Packet &packet, Transform &transform)
	{
		return packet >> transform.m_position >> transform.m_rotation >> transform.m_scaling;
	}

	std::string Transform::ToString() const
	{
		std::stringstream result;
		result << "Transform(" << m_position << ", " << m_rotation << ", " << m_scaling << ")";
		return result.str();
	}
}
