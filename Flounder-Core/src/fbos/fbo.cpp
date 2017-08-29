#include "fbo.hpp"

namespace flounder
{
	fbo::fbo(const int &width, const int &height, const depthbuffer &depthBufferType, const bool &useColourBuffer, const int &attachments, const bool &linearFiltering, const bool &wrapTextures, const bool &clampEdge, const bool &alphaChannel, const bool &antialiased, const int &samples)
	{
		m_width = width;
		m_height = height;
		m_fitToScreen = false;
		m_sizeScalar = 0.0f;

		m_depthBufferType = depthBufferType;
		m_useColourBuffer = useColourBuffer;
		m_attachments = attachments;
		m_linearFiltering = linearFiltering;
		m_wrapTextures = wrapTextures;
		m_clampEdge = clampEdge;
		m_alphaChannel = alphaChannel;
		m_antialiased = antialiased;

		m_samples = samples;

#if 0
		m_frameBuffer = nullptr;
		m_colourTexture = new GLuint[m_attachments];
		m_depthTexture = nullptr;
		m_depthBuffer = nullptr;
		m_colourBuffer = new GLuint[m_attachments];
		m_drawBuffers = new GLenum[m_attachments];
#endif

		initialize();
	}

	fbo::fbo(const bool &fitToScreen, const float &sizeScalar, const depthbuffer &depthBufferType, const bool &useColourBuffer, const int &attachments, const bool &linearFiltering, const bool &wrapTextures, const bool &clampEdge, const bool &alphaChannel, const bool &antialiased, const int &samples)
	{
		m_width = static_cast<int>(display::get()->getWidth() * sizeScalar);
		m_height = static_cast<int>(display::get()->getHeight() * sizeScalar);
		m_fitToScreen = fitToScreen;
		m_sizeScalar = sizeScalar;

		m_depthBufferType = depthBufferType;
		m_useColourBuffer = useColourBuffer;
		m_attachments = attachments;
		m_linearFiltering = linearFiltering;
		m_wrapTextures = wrapTextures;
		m_clampEdge = clampEdge;
		m_alphaChannel = alphaChannel;
		m_antialiased = antialiased;

		m_samples = samples;

#if 0
		m_frameBuffer = nullptr;
		m_colourTexture = new GLuint[m_attachments];
		m_depthTexture = nullptr;
		m_depthBuffer = nullptr;
		m_colourBuffer = new GLuint[m_attachments];
		m_drawBuffers = new GLenum[m_attachments];
#endif

		initialize();
	}

	fbo::~fbo()
	{
		clear();

#if 0
		delete[] m_colourTexture;
		delete[] m_colourBuffer;
#endif
	}

	void fbo::bindFrameBuffer()
	{
		updateSize();
#if 0
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
		glViewport(0, 0, m_width, m_height);
#endif
	}

	void fbo::unbindFrameBuffer()
	{
#if 0
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, display::get()->getWidth(), display::get()->getHeight());
#endif
	}

	void fbo::updateSize()
	{
		if (m_fitToScreen)
		{
			int displayWidth = display::get()->getWidth();
			int displayHeight = display::get()->getHeight();
			int reverseWidth = static_cast<int>(displayWidth * m_sizeScalar);
			int reverseHeight = static_cast<int>(displayHeight * m_sizeScalar);

			if (displayWidth == 0 || displayHeight == 0)
			{
				return;
			}

			if (m_width != reverseWidth || m_height != reverseHeight)
			{
				int newWidth = static_cast<int>(displayWidth * m_sizeScalar);
				int newHeight = static_cast<int>(displayHeight * m_sizeScalar);

				//	if (newWidth < FlounderFBOs.get().getMaxFBOSize() && newHeight < FlounderFBOs.get().getMaxFBOSize()) { // TODO: Fix this ghetto way of fixing the creation of millions of FBOs on old PCs.
				m_width = newWidth;
				m_height = newHeight;
				//	}

				limitFBOSize();
				clear();

				//	std::cout << "Recreating FBO: width: " << m_width << ", and height: " << m_height << std::endl;
				initialize();
			}
		}
	}

	void fbo::blitToScreen()
	{
#if 0
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		GLenum targets[1] = {GL_BACK};
		glDrawBuffers(1, targets);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer);
		glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, display::get()->getWidth(), display::get()->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
#endif
	}

	void fbo::setSamples(const int samples)
	{
		if (m_samples != samples)
		{
			clear();
			//	std::cout << "Recreating FBO: width: " << m_width << ", and height: " << m_height << std::endl;
			initialize();
		}

		m_samples = samples;
	}

	void fbo::setSizeScalar(const float &sizeScalar)
	{
		if (m_fitToScreen && m_sizeScalar == sizeScalar)
		{
			return;
		}

		m_sizeScalar = sizeScalar;
		m_fitToScreen = true;
		updateSize();
	}

	void fbo::setSize(const int &width, const int &height)
	{
		if (!m_fitToScreen && m_width == width && m_height == height)
		{
			return;
		}

		m_width = width;
		m_height = height;
		m_fitToScreen = false;
		updateSize();
	}

	void fbo::resolveFBO(fbo *source, fbo *output)
	{
		if (source->m_attachments != output->m_attachments && output->m_hasGivenResolveError != output->m_hasGivenResolveError)
		{
			source->m_hasGivenResolveError = true;
			output->m_hasGivenResolveError = true;
			//	std::cout << "Warning, resolving two FBO's (" << source << ", " << output << ") with different attachment sizes, be warned this may not work properly instead use resolveFBO(int readBuffer, int drawBuffer, FBO outputFBO)." << std::endl;
		}

		for (int a = 0; a < source->m_attachments; a++)
		{
			resolveFBO(source, a, a, output);
		}
	}

	void fbo::resolveFBO(fbo *source, const int readBuffer, const int drawBuffer, fbo *output)
	{
		output->updateSize();

#if 0
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, output->m_frameBuffer);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, source->m_frameBuffer);

		glReadBuffer(GL_COLOR_ATTACHMENT0 + readBuffer);
		GLenum targets[1] = {static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + drawBuffer)};
		glDrawBuffers(1, targets);
		glBlitFramebuffer(0, 0, source->m_width, source->m_height, 0, 0, output->m_width, output->m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
#endif
		output->unbindFrameBuffer();
	}

	void fbo::initialize()
	{
#if 0
		glGenFramebuffers(1, &m_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
#endif

		if (m_useColourBuffer)
		{
			determineDrawBuffers();
		}
		else
		{
#if 0
			GLenum targets[1] = {GL_FALSE};
			glDrawBuffers(1, targets);
#endif
		}

		limitFBOSize();

		if (!m_antialiased)
		{
			if (m_useColourBuffer)
			{
				for (int i = 0; i < m_attachments; i++)
				{
#if 0
					createTextureAttachment(GL_COLOR_ATTACHMENT0 + i);
#endif
				}
			}

			if (m_depthBufferType == DepthRenderBuffer)
			{
				createDepthBufferAttachment();
			}
			else if (m_depthBufferType == DepthTexture)
			{
				createDepthTextureAttachment();
			}
		}
		else
		{
			for (int i = 0; i < m_attachments; i++)
			{
#if 0
				attachMultisampleColourBuffer(GL_COLOR_ATTACHMENT0 + i);
#endif
			}

			createDepthBufferAttachment();
		}

		unbindFrameBuffer();
	}

	void fbo::determineDrawBuffers()
	{
#if 0
		glDrawBuffers(m_attachments, m_drawBuffers);
#endif
	}

	void fbo::limitFBOSize()
	{
		int maxSize = 0;

#if 0
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, &maxSize);
#endif

		m_width = __min(maxSize, m_width);
		m_height = __min(maxSize, m_height);
	}

	void fbo::createTextureAttachment(const int attachment)
	{
#if 0
		glGenTextures(1, &m_colourTexture[attachment - GL_COLOR_ATTACHMENT0]);
		glBindTexture(GL_TEXTURE_2D, m_colourTexture[attachment - GL_COLOR_ATTACHMENT0]);
		glTexImage2D(GL_TEXTURE_2D, 0, m_alphaChannel ? GL_RGBA : GL_RGB, m_width, m_height, 0, m_alphaChannel ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_linearFiltering ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_linearFiltering ? GL_LINEAR : GL_NEAREST);

		if (m_wrapTextures)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_clampEdge ? GL_CLAMP_TO_EDGE : GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_clampEdge ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_colourTexture[attachment - GL_COLOR_ATTACHMENT0], 0);
#endif
	}

	void fbo::createDepthBufferAttachment()
	{
#if 0
		glGenRenderbuffers(1, &m_depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);

		if (m_antialiased)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH_COMPONENT24, m_width, m_height);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
		}

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
#endif
	}

	void fbo::createDepthTextureAttachment()
	{
#if 0
		glGenTextures(1, &m_depthTexture);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
#endif
	}

	void fbo::attachMultisampleColourBuffer(const int attachment)
	{
#if 0
		glGenRenderbuffers(1, &m_colourBuffer[attachment - GL_COLOR_ATTACHMENT0]);
		glBindRenderbuffer(GL_RENDERBUFFER, m_colourBuffer[attachment - GL_COLOR_ATTACHMENT0]);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, m_alphaChannel ? GL_RGBA8 : GL_RGB8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_colourBuffer[attachment - GL_COLOR_ATTACHMENT0]);
#endif
	}

	void fbo::clear()
	{
#if 0
		glDeleteFramebuffers(1, &m_frameBuffer);
		glDeleteTextures(m_attachments, m_colourTexture);
		glDeleteTextures(1, &m_depthTexture);
		glDeleteRenderbuffers(1, &m_depthBuffer);
		glDeleteRenderbuffers(m_attachments, m_colourBuffer);
#endif
	}
}
