#include "ipostfilter.hpp"

namespace flounder
{
	ipostfilter::ipostfilter(const std::string &filterName, const std::string &fragmentShader, fbo *fbo)
	{
		m_shader = new shader(filterName, 2,
			shadertype(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/filters/default.vert.spv"),
			shadertype(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader)
		);
		m_fbo = fbo;
		m_model = new model("res/models/filter.obj");
	}

	ipostfilter::ipostfilter(const std::string &filterName, const std::string &fragmentShader)
	{
		m_shader = new shader(filterName, 2,
			shadertype(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/filters/default.vert.spv"),
			shadertype(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader)
		);
		m_fbo = new fbo(true, 1.0f);
		m_model = new model("res/models/filter.obj");
	}

	ipostfilter::ipostfilter(shader *shader, fbo *fbo)
	{
		m_shader = shader;
		m_fbo = fbo;
		m_model = new model("res/models/filter.obj");
	}

	ipostfilter::ipostfilter(shader *shader)
	{
		m_shader = shader;
		m_fbo = new fbo(true, 1.0f);
		m_model = new model("res/models/filter.obj");
	}

	ipostfilter::~ipostfilter()
	{
		delete m_shader;
		delete m_fbo;
	}

	void ipostfilter::applyFilter(const int n_args, ...)
	{
		va_list args;
		va_start(args, n_args);
		applyFilter(n_args, args);
		va_end(args);
	}

	void ipostfilter::applyFilter(const int n_args, va_list args)
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
