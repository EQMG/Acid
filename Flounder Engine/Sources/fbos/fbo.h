#pragma once

#include <iostream>
#include <cmath>
#include <GL/glew.h>

#include "../devices/display.h"

#include "depthbuffer.h"
#include "fbobuilder.h"

namespace flounder {
	/// <summary>
	/// A class that represents a OpenGL Frame Buffer object.
	/// </summary>
	class fbo
	{
	private:
		depthbuffer m_depthBufferType;
		bool m_useColourBuffer;
		bool m_linearFiltering;
		bool m_wrapTextures;
		bool m_clampEdge;
		bool m_alphaChannel;
		bool m_antialiased;
		int m_samples;
		int m_width;
		int m_height;
		int m_attachments;
		bool m_fitToScreen;
		float m_sizeScalar;

		GLuint m_frameBuffer;
		GLuint *m_colourTexture;
		GLuint m_depthTexture;
		GLuint m_depthBuffer;
		GLuint *m_colourBuffer;
		GLenum *m_drawBuffers;

		bool m_hasGivenResolveError;

	public:
		/// <summary>
		/// A new OpenGL FBO object.
		/// </summary>
		/// <param name="builder"> The fbo builder to load the fbo from. </param>
		fbo(const fbobuilder *builder);

		/// <summary>
		/// Deconstructor for the fbo.
		/// </summary>
		~fbo();
	private:
		/// <summary>
		/// Initializes the FBO.
		/// </summary>
		void initialize();

		void determineDrawBuffers();

		void limitFBOSize();

		void createTextureAttachment(const int attachment);

		void createDepthBufferAttachment();

		void createDepthTextureAttachment();

		void attachMultisampleColourBuffer(const int attachment);

		void clear();
	public:
		/// <summary>
		/// Binds the FBO so it can be rendered too.
		/// </summary>
		void bindFrameBuffer();

		/// <summary>
		/// Unbinds the FBO so that other rendering objects can be used.
		/// </summary>
		void unbindFrameBuffer();

		/// <summary>
		/// Updates the FBO size if {@code fitToScreen}.
		/// </summary>
		void updateSize();

		/// <summary>
		/// Renders the colour buffer to the display.
		/// </summary>
		void blitToScreen();

		inline depthbuffer getDepthBufferType() { return m_depthBufferType; }

		/// <summary>
		/// Gets the number of attachments in this FBO.
		/// </summary>
		/// <returns> The number of attachments in this FBO. </returns>
		inline int getAttachments() { return m_attachments; }

		/// <summary>
		/// Gets the number of antialiasing samples.
		/// </summary>
		/// <returns> The number of antialiasing samples. </returns>
		inline int getSamples() { return m_samples; }

		/// <summary>
		/// Sets the number antialiasing samples, and recreates the FBO.
		/// </summary>
		/// <param name="samples"> The number of antialiasing samples. </param>
		virtual void setSamples(const int samples);

		inline GLuint getFrameBuffer() { return m_frameBuffer; }

		inline GLuint *getColourTexture() { return m_colourTexture; }

		/// <summary>
		/// Gets the depth texture.
		/// </summary>
		/// <returns> The OpenGL depth texture id. </returns>
		inline GLuint getDepthTexture() { return m_depthTexture; }

		inline GLuint setDepthTexture(int depthTexture) { m_depthTexture = depthTexture; }

		inline GLuint getDepthBuffer() { return m_depthBuffer; }

		inline GLuint *getColourBuffer() { return m_colourBuffer; }

		/// <summary>
		/// Blits this FBO and all attachments to another FBO.
		/// </summary>
		/// <param name="source"> The source fbo </param>
		/// <param name="output"> The other FBO to blit to. </param>
		static void resolveFBO(fbo *source, fbo *output);

		/// <summary>
		/// Blits this FBO attachment to another FBO attachment.
		/// </summary>
		/// <param name="source"> The source fbo </param>
		/// <param name="readBuffer"> The colour attachment to be read from. </param>
		/// <param name="drawBuffer"> The colour draw buffer to be written to. </param>
		/// <param name="output"> The other FBO to blit to. </param>
		static void resolveFBO(fbo *source, const int readBuffer, const int drawBuffer, fbo *output);
	};
}
