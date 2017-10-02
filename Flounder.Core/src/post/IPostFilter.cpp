#include "IPostFilter.hpp"

namespace Flounder
{
	IPostFilter::IPostFilter(const std::string &filterName, const std::string &fragmentShader, Fbo *fbo) :
		m_shader(new Shader(filterName, {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/filters/default.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader)
		})),
		m_fbo(fbo),
		m_model(new Model("res/models/rectangle_flat.obj"))
	{
	}

	IPostFilter::IPostFilter(Shader *shader, Fbo *fbo) :
		m_shader(shader),
		m_fbo(fbo),
		m_model(new Model("res/models/rectangle_flat.obj"))
	{
	}

	IPostFilter::IPostFilter(Shader *shader) :
		m_shader(shader),
		m_fbo(new Fbo(true, 1.0f)),
		m_model(new Model("res/models/rectangle_flat.obj"))
	{
	}

	IPostFilter::~IPostFilter()
	{
		delete m_shader;
		delete m_fbo;
	}

	void IPostFilter::ApplyFilter(const int n_args, ...)
	{
		va_list args;
		va_start(args, n_args);
		ApplyFilter(n_args, args);
		va_end(args);
	}

	void IPostFilter::ApplyFilter(const int n_args, va_list args)
	{
#if 0
		if (m_fbo != nullptr)
		{
			m_fbo->bindFrameBuffer();
			renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f, 1.0f);
		}

		m_shader->start();

		storeValues();

		renderer::get()->disableDepthTesting();
		renderer::get()->cullBackFaces(true);
		renderer::get()->bindVAO(m_model->getVaoID(), 2, 0, 1);

		for (int i = 0; i < n_args; i++)
		{
			renderer::get()->bindTexture(texture, GL_TEXTURE_2D, i);
		}

		renderer::get()->renderElements(GL_TRIANGLES, GL_UNSIGNED_INT, m_model->getVaoLength()); // Render post filter.

		renderer::get()->unbindVAO(2, 0, 1);
		m_shader->stop();
		renderer::get()->disableBlending();
		renderer::get()->enableDepthTesting();

		if (m_fbo != nullptr)
		{
			m_fbo->unbindFrameBuffer();
		}
#endif
	}
}
