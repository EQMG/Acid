#include "material.h"

namespace flounder
{
	material::material(const std::string &file)
	{
		m_file = file;

		m_specularCoefficient = 100.0f;
		m_ambient = new colour(0.2f, 0.2f, 0.2f);
		m_diffuse = new colour(0.3f, 1.0f, 1.0f);
		m_specular = new colour(1.0f, 1.0f, 1.0f);

		m_albedo = nullptr;
		m_normals = nullptr;

		loadFromFile();
	}

	material::~material()
	{
		delete m_ambient;
		delete m_diffuse;
		delete m_specular;

		delete m_albedo;
		delete m_normals;
	}

	void material::loadFromFile()
	{
		std::string fileLoaded = helperfile::readTextFile(m_file);
		std::vector<std::string> lines = helperstring::split(fileLoaded, "\n");

		for (std::vector<std::string>::iterator it = lines.begin(); it < lines.end(); it++)
		{
		}

	}
}
