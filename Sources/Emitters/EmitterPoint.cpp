#include "EmitterPoint.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
	EmitterPoint::EmitterPoint(const Transform &localTransform) :
		Emitter(localTransform),
		m_point(Vector3())
	{
	}

	void EmitterPoint::Start()
	{
	}

	void EmitterPoint::Update()
	{
	}

	void EmitterPoint::Decode(const Metadata &metadata)
	{
		m_point = metadata.GetChild<Vector3>("Point");
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
	}

	void EmitterPoint::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Vector3>("Point", m_point);
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
	}

	Vector3 EmitterPoint::GeneratePosition() const
	{
		return m_point;
	}
}
