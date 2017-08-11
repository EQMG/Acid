#include "renderer.h"

namespace flounder
{
	renderer::renderer() :
		imodule()
	{
		m_managerRender = nullptr;

		m_cullingBackFace = false;
		m_depthMask = true;
		m_inWireframe = false;
		m_isAlphaBlending = false;
		m_additiveBlending = false;
		m_antialiasing = false;

		createGraphicsPipeline();
	}

	renderer::~renderer()
	{
		delete m_managerRender;
	}

	void renderer::update()
	{
		if (m_managerRender != nullptr)
		{
			m_managerRender->render();
		}
	}

	void renderer::prepareNewRenderParse(colour *colour)
	{
		prepareNewRenderParse(colour->m_r, colour->m_g, colour->m_b, colour->m_a);
	}

	void renderer::prepareNewRenderParse(const float &r, const float &g, const float &b, const float &a)
	{
#if 0
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
		disableBlending();
		cullBackFaces(true);
		enableDepthTesting();
	}

	void renderer::cullBackFaces(const bool &cull)
	{
		if (cull && !m_cullingBackFace)
		{
#if 0
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
#endif
			m_cullingBackFace = true;
		}
		else if (!cull && m_cullingBackFace)
		{
#if 0
			glDisable(GL_CULL_FACE);
#endif
			m_cullingBackFace = false;
		}
	}

	void renderer::enableDepthTesting()
	{
#if 0
		glEnable(GL_DEPTH_TEST);
#endif
	}

	void renderer::disableDepthTesting()
	{
#if 0
		glDisable(GL_DEPTH_TEST);
#endif
	}

	void renderer::depthMask(const bool &depthMask)
	{
#if 0
		glDepthMask(depthMask);
#endif
		m_depthMask = depthMask;
	}

	bool renderer::isInWireframe()
	{
		return m_inWireframe;
	}

	void renderer::goWireframe(const bool &goWireframe)
	{
		if (goWireframe && !m_inWireframe)
		{
#if 0
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
			m_inWireframe = true;
		}
		else if (!goWireframe && m_inWireframe)
		{
#if 0
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
			m_inWireframe = false;
		}
	}

	void renderer::enableAlphaBlending()
	{
		if (!m_isAlphaBlending)
		{
#if 0
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
			m_isAlphaBlending = true;
			m_additiveBlending = false;
		}
	}

	void renderer::enableAdditiveBlending()
	{
		if (!m_additiveBlending)
		{
#if 0
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
#endif
			m_additiveBlending = true;
			m_isAlphaBlending = false;
		}
	}

	void renderer::disableBlending()
	{
		if (m_isAlphaBlending || m_additiveBlending)
		{
#if 0
			glDisable(GL_BLEND);
#endif
			m_isAlphaBlending = false;
			m_additiveBlending = false;
		}
	}

	void renderer::antialias(const bool &enable)
	{
		if (enable && !m_antialiasing)
		{
			//	glEnable(GL_MULTISAMPLE);
			m_antialiasing = true;
		}
		else if (!enable && m_antialiasing)
		{
			//	glDisable(GL_MULTISAMPLE);
			m_antialiasing = false;
		}
	}

	void renderer::bindVAO(const int &vaoID, const int n_args, ...)
	{
#if 0
		glBindVertexArray(vaoID);
#endif

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			int attribute = va_arg(ap, int);
#if 0
			glEnableVertexAttribArray(attribute);
#endif
		}

		va_end(ap);
	}

	void renderer::unbindVAO(const int n_args, ...)
	{
		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			int attribute = va_arg(ap, int);
#if 0
			glDisableVertexAttribArray(attribute);
#endif
		}

		va_end(ap);

#if 0
		glBindVertexArray(0);
#endif
	}

	void renderer::scissorEnable(const int &x, const int &y, const int &width, const int &height)
	{
#if 0
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, y, width, height);
#endif
	}

	void renderer::scissorDisable()
	{
#if 0
		glDisable(GL_SCISSOR_TEST);
#endif
	}

	void renderer::bindTexture(texture *texture, const int &bankID)
	{
#if 0
		glActiveTexture(GL_TEXTURE0 + bankID);

		switch (texture->getTextureType())
		{
		case texture::typeTexture2D:
			glBindTexture(GL_TEXTURE_2D, texture->getGlTexture().m_textureID);
			break;
		case texture::typeTextureCubeMap:
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture->getGlTexture().m_textureID);
			break;
		}
#endif
	}

	void renderer::bindTexture(const int &textureID, const int &glTarget, const int &bankID)
	{
#if 0
		glActiveTexture(GL_TEXTURE0 + bankID);
		glBindTexture(glTarget, textureID);
#endif
	}

	void renderer::bindTextureLOD(const int &textureID, const int &lodBias, const int &bankID)
	{
#if 0
		glActiveTexture(GL_TEXTURE0 + bankID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, lodBias);
		glActiveTexture(0);
#endif
	}

	void renderer::renderArrays(const int &glMode, const int &glLength)
	{
#if 0
		glDrawArrays(glMode, 0, glLength);
#endif
	}

	void renderer::renderElements(const int &glMode, const int &glType, const int &glLength)
	{
#if 0
		glDrawElements(glMode, glLength, glType, 0);
#endif
	}

	void renderer::renderInstanced(const int &glMode, const int &glLength, const int &glPrimCount)
	{
#if 0
		glDrawArraysInstanced(glMode, 0, glLength, glPrimCount);
#endif
	}

	void renderer::createGraphicsPipeline()
	{
	}
}
