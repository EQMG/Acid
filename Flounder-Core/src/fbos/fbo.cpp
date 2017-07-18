#include "fbo.h"

namespace flounder {
	fbo::builder::builder()
	{
		m_fbo = new fbo(this);
	}

	fbo::builder::~builder()
	{
	}

	fbo::builder *fbo::builder::setSize(const int &width, const int &height)
	{
		m_fbo->m_width = width;
		m_fbo->m_height = height;
		return this;
	}

	fbo::builder *fbo::builder::depthBuffer(const depthbuffer &type)
	{
		m_fbo->m_depthBufferType = type;
		return this;
	}

	fbo::builder *fbo::builder::noColourBuffer()
	{
		m_fbo->m_useColourBuffer = false;
		return this;
	}

	fbo::builder *fbo::builder::nearestFiltering()
	{
		m_fbo->m_linearFiltering = false;
		return this;
	}

	fbo::builder *fbo::builder::disableTextureWrap()
	{
		m_fbo->m_wrapTextures = false;
		return this;
	}

	fbo::builder *fbo::builder::repeatTexture()
	{
		m_fbo->m_clampEdge = false;
		return this;
	}

	fbo::builder *fbo::builder::withAlphaChannel(const bool &alpha)
	{
		m_fbo->m_alphaChannel = alpha;
		return this;
	}

	fbo::builder *fbo::builder::antialias(const int &samples)
	{
		m_fbo->m_antialiased = true;
		m_fbo->m_samples = samples;
		return this;
	}

	fbo::builder *fbo::builder::attachments(const int &attachments)
	{
		if (m_fbo->m_attachments != attachments)
		{
			delete[] m_fbo->m_colourTexture;
			delete[] m_fbo->m_colourBuffer;
			delete[] m_fbo->m_drawBuffers;
			m_fbo->m_attachments = attachments;
			m_fbo->m_colourTexture = new GLuint[attachments];
			m_fbo->m_colourBuffer = new GLuint[attachments];
			m_fbo->m_drawBuffers = new GLenum[attachments];
		}

		return this;
	}

	fbo::builder *fbo::builder::fitToScreen(const float &sizeScalar)
	{
		m_fbo->m_fitToScreen = true;
		m_fbo->m_sizeScalar = sizeScalar;
		return this;
	}

	flounder::fbo *fbo::builder::create()
	{
		for (int i = 0; i < m_fbo->m_attachments; i++)
		{
			m_fbo->m_drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		m_fbo->m_hasGivenResolveError = false;

		m_fbo->initialize();

		//delete this;
		return m_fbo;
	}

	fbo::fbo(builder *builder)
	{
		m_builder = builder;

		m_depthBufferType = NONE;
		m_useColourBuffer = true;
		m_linearFiltering = true;
		m_wrapTextures = true;
		m_clampEdge = true;
		m_alphaChannel = false;
		m_antialiased = false;
		m_samples = 0;
		m_width = display::get()->getWidth();
		m_height = display::get()->getHeight();
		m_attachments = 1;
		m_fitToScreen = true;
		m_sizeScalar = 1.0f;

		m_frameBuffer = NULL;
		m_colourTexture = new GLuint[m_attachments];
		m_depthTexture = NULL;
		m_depthBuffer = NULL;
		m_colourBuffer = new GLuint[m_attachments];
		m_drawBuffers = new GLenum[m_attachments];
	}

	fbo::~fbo()
	{
		clear();

		delete m_builder;

		//delete m_frameBuffer;
		delete[] m_colourTexture;
		//delete m_depthTexture;
		//delete m_depthBuffer;
		delete[] m_colourBuffer;
	}

	fbo::builder *fbo::newFBO()
	{
		return new builder();
	}

	void fbo::bindFrameBuffer()
	{
		updateSize();
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
		glViewport(0, 0, m_width, m_height);
	}

	void fbo::unbindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, display::get()->getWidth(), display::get()->getHeight());
	}

	void fbo::updateSize()
	{
		if (m_fitToScreen)
		{
			int displayWidth = display::get()->getWidth();
			int displayHeight = display::get()->getHeight();
			int reverseWidth = (int)(displayWidth * m_sizeScalar);
			int reverseHeight = (int)(displayHeight * m_sizeScalar);

			if (displayWidth == 0 || displayHeight == 0)
			{
				return;
			}

			if (m_width != reverseWidth || m_height != reverseHeight)
			{
				int newWidth = (int) (displayWidth * m_sizeScalar);
				int newHeight = (int) (displayHeight * m_sizeScalar);
				
				//	if (newWidth < FlounderFBOs.get().getMaxFBOSize() && newHeight < FlounderFBOs.get().getMaxFBOSize()) { // TODO: Fix this ghetto way of fixing the creation of millions of FBOs on old PCs.
				m_width = newWidth;
				m_height = newHeight;
				//	}

				limitFBOSize();
				clear();

				std::cout << "Recreating FBO: width: " << m_width << ", and height: " << m_height << std::endl;
				initialize();
			}
		}
	}

	void fbo::blitToScreen()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		GLenum targets[1] = { GL_BACK };
		glDrawBuffers(1, targets);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer);
		glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, display::get()->getWidth(), display::get()->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void fbo::setSamples(const int samples)
	{
		if (m_samples != samples)
		{
			clear();
			std::cout << "Recreating FBO: width: " << m_width << ", and height: " << m_height << std::endl;
			initialize();
		}

		m_samples = samples;
	}

	void fbo::resolveFBO(fbo *source, fbo *output)
	{
		if (source->m_attachments != output->m_attachments && output->m_hasGivenResolveError != output->m_hasGivenResolveError) 
		{
			source->m_hasGivenResolveError = true;
			output->m_hasGivenResolveError = true;
			std::cout << "Warning, resolving two FBO's (" << source << ", " << output << ") with different attachment sizes, be warned this may not work properly instead use resolveFBO(int readBuffer, int drawBuffer, FBO outputFBO)." << std::endl;
		}

		for (int a = 0; a < source->m_attachments; a++)
		{
			resolveFBO(source, a, a, output);
		}
	}

	void fbo::resolveFBO(fbo *source, const int readBuffer, const int drawBuffer, fbo *output)
	{
		output->updateSize();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, output->m_frameBuffer);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, source->m_frameBuffer);

		glReadBuffer(GL_COLOR_ATTACHMENT0 + readBuffer);
		GLenum targets[1] = { static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + drawBuffer) };
		glDrawBuffers(1, targets);
		glBlitFramebuffer(0, 0, source->m_width, source->m_height, 0, 0, output->m_width, output->m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		output->unbindFrameBuffer();
	}

	void fbo::initialize()
	{
		glGenFramebuffers(1, &m_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

		if (m_useColourBuffer) 
		{
			determineDrawBuffers();
		}
		else
		{
			GLenum targets[1] = { GL_FALSE };
			glDrawBuffers(1, targets);
		}

		limitFBOSize();

		if (!m_antialiased) 
		{
			if (m_useColourBuffer)
			{
				for (int i = 0; i < m_attachments; i++)
				{
					createTextureAttachment(GL_COLOR_ATTACHMENT0 + i);
				}
			}

			if (m_depthBufferType == RENDER_BUFFER)
			{
				createDepthBufferAttachment();
			}
			else if (m_depthBufferType == TEXTURE)
			{
				createDepthTextureAttachment();
			}
		}
		else
		{
			for (int i = 0; i < m_attachments; i++)
			{
				attachMultisampleColourBuffer(GL_COLOR_ATTACHMENT0 + i);
			}

			createDepthBufferAttachment();
		}

		unbindFrameBuffer();
	}

	void fbo::determineDrawBuffers()
	{
		glDrawBuffers(m_attachments, m_drawBuffers);
	}

	void fbo::limitFBOSize()
	{
		int maxSize = 0;
#ifdef FLOUNDER_PLATFORM_WEB
#define GL_MAX_RENDERBUFFER_SIZE_EXT      0x84E8
#endif
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, &maxSize);

		m_width = __min(maxSize, m_width);
		m_height = __min(maxSize, m_height);
	}

	void fbo::createTextureAttachment(const int attachment)
	{
		glGenTextures(1, &m_colourTexture[attachment - GL_COLOR_ATTACHMENT0]);
		glBindTexture(GL_TEXTURE_2D, m_colourTexture[attachment - GL_COLOR_ATTACHMENT0]);
		glTexImage2D(GL_TEXTURE_2D, 0, m_alphaChannel ? GL_RGBA : GL_RGB, m_width, m_height, 0, m_alphaChannel ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_linearFiltering ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_linearFiltering ? GL_LINEAR : GL_NEAREST);

		if (m_wrapTextures)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_clampEdge ? GL_CLAMP_TO_EDGE : GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_clampEdge ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_colourTexture[attachment - GL_COLOR_ATTACHMENT0], 0);
	}

	void fbo::createDepthBufferAttachment()
	{
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
	}

	void fbo::createDepthTextureAttachment()
	{
		glGenTextures(1, &m_depthTexture);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
	}

	void fbo::attachMultisampleColourBuffer(const int attachment)
	{
		glGenRenderbuffers(1, &m_colourBuffer[attachment - GL_COLOR_ATTACHMENT0]);
		glBindRenderbuffer(GL_RENDERBUFFER, m_colourBuffer[attachment - GL_COLOR_ATTACHMENT0]);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, m_alphaChannel ? GL_RGBA8 : GL_RGB8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_colourBuffer[attachment - GL_COLOR_ATTACHMENT0]);
	}

	void fbo::clear()
	{
		glDeleteFramebuffers(1, &m_frameBuffer);
		glDeleteTextures(m_attachments, m_colourTexture);
		glDeleteTextures(1, &m_depthTexture);
		glDeleteRenderbuffers(1, &m_depthBuffer);
		glDeleteRenderbuffers(m_attachments, m_colourBuffer);
	}
}
