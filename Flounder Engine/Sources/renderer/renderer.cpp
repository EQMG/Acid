#include "renderer.h"

namespace flounder {
	renderer::renderer()
		: imodule()
	{
		m_renderermaster = NULL;
	}

	renderer::~renderer()
	{
		delete m_renderermaster;
	}

	void renderer::loadRendererMaster(irenderermaster *renderermaster)
	{
		m_renderermaster = renderermaster;
	}

	void renderer::init()
	{
	}

	void renderer::update()
	{
		if (m_renderermaster != NULL)
		{
			m_renderermaster->render();
		}
	}

	void renderer::prepareNewRenderParse(colour *colour)
	{
		prepareNewRenderParse(colour->r, colour->g, colour->b);
	}

	void renderer::prepareNewRenderParse(float r, float g, float b)
	{
		glClearColor(r, g, b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		disableBlending();
		cullBackFaces(true);
		enableDepthTesting();
	}

	void renderer::cullBackFaces(bool cull)
	{
		if (cull && !m_cullingBackFace) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			m_cullingBackFace = true;
		}
		else if (!cull && m_cullingBackFace) {
			glDisable(GL_CULL_FACE);
			m_cullingBackFace = false;
		}
	}

	void renderer::enableDepthTesting()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void renderer::disableDepthTesting()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void renderer::depthMask(bool depthMask)
	{
		m_depthMask = depthMask;
		glDepthMask(depthMask);
	}

	bool renderer::isInWireframe()
	{
		return m_inWireframe;
	}

	void renderer::goWireframe(bool goWireframe)
	{
		if (goWireframe && !m_inWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			m_inWireframe = true;
		}
		else if (!goWireframe && m_inWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			m_inWireframe = false;
		}
	}

	void renderer::enableAlphaBlending()
	{
		if (!m_isAlphaBlending)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			m_isAlphaBlending = true;
			m_additiveBlending = false;
		}
	}

	void renderer::enableAdditiveBlending()
	{
		if (!m_additiveBlending)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			m_additiveBlending = true;
			m_isAlphaBlending = false;
		}
	}

	void renderer::disableBlending()
	{
		if (m_isAlphaBlending || m_additiveBlending)
		{
			glDisable(GL_BLEND);
			m_isAlphaBlending = false;
			m_additiveBlending = false;
		}
	}

	void renderer::antialias(bool enable)
	{
		if (enable && !m_antialiasing)
		{
			glEnable(GL_MULTISAMPLE);
			m_antialiasing = true;
		}
		else if (!enable && m_antialiasing)
		{
			glDisable(GL_MULTISAMPLE);
			m_antialiasing = false;
		}
	}

	void renderer::bindVAO(int vaoID, int n_args, ...)
	{
		glBindVertexArray(vaoID);

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			int attribute = va_arg(ap, int);
			glEnableVertexAttribArray(attribute);
		}

		va_end(ap);
	}

	void renderer::unbindVAO(int n_args, ...)
	{
		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			int attribute = va_arg(ap, int);
			glDisableVertexAttribArray(attribute);
		}

		va_end(ap);

		glBindVertexArray(0);
	}

	void renderer::scissor(int x, int y, int width, int height)
	{
		glScissor(x, y, width, height);
	}

	void renderer::bindTexture(texture *texture, int bankID)
	{
		if (texture == NULL || texture->getTextureID() == -1) 
		{
			return;
		}

		glActiveTexture(GL_TEXTURE0 + bankID);
		glBindTexture(texture->getGlType(), texture->getTextureID());
	}

	void renderer::bindTexture(int textureID, int glTarget, int bankID)
	{
		if (textureID == -1) 
		{
			return;
		}

		glActiveTexture(GL_TEXTURE0 + bankID);
		glBindTexture(glTarget, textureID);
	}

	void renderer::bindTextureLOD(int textureID, int lodBias, int bankID)
	{
		if (textureID == -1) 
		{
			return;
		}

		glActiveTexture(GL_TEXTURE0 + bankID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, lodBias);
		glActiveTexture(0);
	}

	void renderer::renderArrays(GLenum glMode, GLsizei glLength)
	{
		glDrawArrays(glMode, 0, glLength);
	}

	void renderer::renderElements(GLenum glMode, GLenum glType, GLsizei glLength)
	{
		glDrawElements(glMode, glLength, glType, 0);
	}

	void renderer::renderInstanced(GLenum glMode, GLsizei glLength, GLsizei glPrimCount)
	{
		glDrawArraysInstanced(glMode, 0, glLength, glPrimCount);
		// glDrawArraysInstancedARB(glMode, 0, glLength, glPrimCount);
	}
}
