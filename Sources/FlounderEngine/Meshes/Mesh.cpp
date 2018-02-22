#include "Mesh.hpp"

#include <cassert>
#include "../Models/Shapes/ShapeSphere.hpp"
#include "../Models/Shapes/ShapeCube.hpp"
#include "../Models/Shapes/ShapeRectangle.hpp"
#include "Helpers/FileSystem.hpp"

namespace Flounder
{
	Mesh::Mesh(Model *model) :
		Component(),
		m_model(model)
	{
		Link<std::string>(0, LINK_GET_RES(GetModel()), LINK_SET(std::string, SetModel(DecodeModelType(v))));
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Update()
	{
	}

	Model *Mesh::DecodeModelType(const std::string &filename)
	{
		auto split = FormatString::Split(filename, "_");

		if (!split.empty() && split.at(0) == "Sphere")
		{
			return ShapeSphere::Resource(filename);
		}
		else if (!split.empty() && split.at(0) == "Cube")
		{
			return ShapeCube::Resource(filename);
		}
		else if (!split.empty() && split.at(0) == "Rectangle")
		{
			return ShapeRectangle::Resource(filename);
		}

		return Model::Resource(filename);
	}
}
