#include "model.h"

namespace flounder {
	model::builder::builder()
	{
		m_model = new model(this);
	}

	model::builder::~builder()
	{
	}

	model::builder *model::builder::setFile(const std::string &file)
	{
		m_model->m_file = file;
		return this;
	}

	flounder::model *model::builder::create()
	{
		m_model->loadFromFile(m_model->m_file);
		m_model->loadToOpenGL();
		return m_model;
	}

	model::model(builder *builder)
	{
		m_builder = builder;

		m_file = "";

		m_vertices = NULL;
		m_textures = NULL;
		m_normals = NULL;
		m_tangents = NULL;
		m_indices = NULL;

		m_vaoID = 0;
		m_vaoLength = 0;
	}

	model::~model()
	{
		//loaders::get()->removeVAO(m_vaoID);

		delete m_builder;

		delete[] m_vertices;
		delete[] m_textures;
		delete[] m_normals;
		delete[] m_tangents;
		delete[] m_indices;
	}

	model::builder *model::newModel()
	{
		return new builder();
	}

	void model::loadFromFile(const std::string &file)
	{
		
	}

	void model::loadToOpenGL()
	{
		m_vaoID = loaders::get()->createVAO();
		loaders::get()->createIndicesVBO(m_vaoID, m_indices);
		loaders::get()->storeDataInVBO(m_vaoID, m_vertices, 0, 3);
		loaders::get()->storeDataInVBO(m_vaoID, m_textures, 1, 2);
		loaders::get()->storeDataInVBO(m_vaoID, m_normals, 2, 3);
		loaders::get()->storeDataInVBO(m_vaoID, m_tangents, 3, 3);

		if (m_indices != NULL)
		{
			m_vaoLength = sizeof(m_indices) / sizeof(*m_indices);
		}
		else
		{
			m_vaoLength = (sizeof(m_vertices) / sizeof(*m_vertices)) / 3;
		}
	}
}