#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <GL/glew.h>

#include "../devices/display.h"

#include "depthbuffer.h"

namespace flounder {
	/// <summary>
	/// A class that represents a OpenGL Frame Buffer object.
	/// </summary>
	class fbo
	{
		/// <summary>
		/// A builder used to set fbo parameters for loading.
		/// </summary>
		class builder
		{
		private:
			fbo *m_fbo;
		public:
			/// <summary>
			/// Creates a new fbo builder.
			/// </summary>
			builder();

			/// <summary>
			/// Deconstructor for the shader builder.
			/// </summary>
			~builder();

			/// <summary>
			/// Sets the size of the fbo.
			/// </summary>
			/// <param name="width"> The fbos's initial width. </param>
			/// <param name="height"> The fbos's initial height. </param>
			/// <returns> This. </returns>
			builder *setSize(const int &width, const int &height);

			/// <summary>
			/// Sets the type of depth buffer to use.
			/// </summary>
			/// <param name="type"> The depth buffer to use.
			/// </param>
			/// <returns> This. </returns>
			builder *depthBuffer(const depthbuffer &type);

			/// <summary>
			/// Disables the colour buffer.
			/// </summary>
			/// <returns> This. </returns>
			builder *noColourBuffer();

			/// <summary>
			/// Sets the texture to not use linear filtering.
			/// </summary>
			/// <returns> This. </returns>
			builder *nearestFiltering();

			/// <summary>
			/// Sets if the textures will even be bothered with wrapping.
			/// </summary>
			/// <returns> This. </returns>
			builder *disableTextureWrap();

			/// <summary>
			/// Sets the texture to repeat.
			/// </summary>
			/// <returns> This. </returns>
			builder *repeatTexture();

			/// <summary>
			/// Enables / disables the alpha channel.
			/// </summary>
			/// <param name="alpha"> If the alpha channel will be enabled.
			/// </param>
			/// <returns> This. </returns>
			builder *withAlphaChannel(const bool &alpha);

			/// <summary>
			/// Sets antialiased to true and adds samples.
			/// </summary>
			/// <param name="samples"> How many MFAA samples should be used on the FBO. Zero disables multisampling.
			/// </param>
			/// <returns> This. </returns>
			builder *antialias(const int &samples);

			/// <summary>
			/// Sets the amount of colour attachments to create.
			/// </summary>
			/// <param name="attachments"> The amount of attachments to create.
			/// </param>
			/// <returns> This. </returns>
			builder *attachments(const int &attachments);

			/// <summary>
			/// Sets if the FBO will be fit to the screen.
			/// </summary>
			/// <param name="sizeScalar"> A scalar factor between the FBO and the screen, enabled when {@code fitToScreen} is enabled. (1.0f disables scalar).
			/// </param>
			/// <returns> This. </returns>
			builder *fitToScreen(const float &sizeScalar);

			/// <summary>
			/// Creates a fbo from the builder.
			/// </summary>
			/// <returns> The created fbo. </returns>
			fbo *create();
		};

	protected:
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

		/// <summary>
		/// A new OpenGL FBO object.
		/// </summary>
		fbo();
	public:
		/// <summary>
		/// Deconstructor for the fbo.
		/// </summary>
		~fbo();

		/// <summary>
		/// Creates a new fbo builder that is used to configure a fbo.
		/// </summary>
		/// <returns> The fbo builder. </returns>
		static builder *newFBO();

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
	};
}
