#include "ShapeConvexHull.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ShapeConvexHull::ShapeConvexHull(const std::vector<Vector3> &data) :
		m_shape(nullptr)
	{
		std::vector<btScalar> scalars = std::vector<btScalar>(data.size() * 3);
		int i = 0;

		for (auto &vector : data)
		{
			scalars[i] = vector.m_x;
			scalars[i + 1] = vector.m_y;
			scalars[i + 2] = vector.m_z;
			i += 3;
		}

		m_shape = new btConvexHullShape(scalars.data(), data.size(), sizeof(btVector3));
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ShapeConvexHull::~ShapeConvexHull()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeConvexHull::Load(LoadedValue *value)
	{
		std::string source = value->GetChild("Source")->GetValue();
	}

	void ShapeConvexHull::Write(LoadedValue *destination)
	{
	}

	void ShapeConvexHull::LoadData(const std::vector<Vector3> &data)
	{
		std::vector<btScalar> scalars = std::vector<btScalar>(data.size() * 3);
		int i = 0;

		for (auto &vector : data)
		{
			scalars[i] = vector.m_x;
			scalars[i + 1] = vector.m_y;
			scalars[i + 2] = vector.m_z;
			i += 3;
		}

		// TODO
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
		m_shape = new btConvexHullShape(scalars.data(), data.size(), sizeof(btVector3));
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}
}
