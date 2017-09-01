#pragma once

#include <iostream>
#include <string>
#include <cmath>

#include "../devices/Display.hpp"
#include "../maths/maths.hpp"

#include "depthbuffer.hpp"

namespace Flounder
{
	/// <summary>
	/// A class that represents a Frame Buffer object.
	/// </summary>
	class Fbo
	{
	private:
		int m_width;
		int m_height;
		bool m_fitToScreen;
		float m_sizeScalar;

		DepthBuffer m_depthBufferType;
		bool m_useColourBuffer;
		int m_attachments;
		bool m_linearFiltering;
		bool m_wrapTextures;
		bool m_clampEdge;
		bool m_alphaChannel;
		bool m_antialiased;

		int m_samples;

#if 0
		GLuint m_frameBuffer;
		GLuint *m_colourTexture;
		GLuint m_depthTexture;
		GLuint m_depthBuffer;
		GLuint *m_colourBuffer;
		GLenum *m_drawBuffers;
#endif

		bool m_hasGivenResolveError;

	public:
		/// <summary>
		/// A new fbo object.
		/// </summary>
		/// <param name="builder"> The fbo builder. </param>
		Fbo(const int &width = 0,
			const int &height = 0,
			const DepthBuffer &depthBufferType = DepthNone,
			const bool &useColourBuffer = true,
			const int &attachments = 1,
			const bool &linearFiltering = true,
			const bool &wrapTextures = false,
			const bool &clampEdge = true,
			const bool &alphaChannel = false,
			const bool &antialiased = false,
			const int &samples = 0);

		Fbo(const bool &fitToScreen = true,
			const float &sizeScalar = 1.0f,
			const DepthBuffer &depthBufferType = DepthNone,
			const bool &useColourBuffer = true,
			const int &attachments = 1,
			const bool &linearFiltering = true,
			const bool &wrapTextures = false,
			const bool &clampEdge = true,
			const bool &alphaChannel = false,
			const bool &antialiased = false,
			const int &samples = 0);

		/// <summary>
		/// Deconstructor for the fbo.
		/// </summary>
		~Fbo();

		/// <summary>
		/// Binds the FBO so it can be rendered too.
		/// </summary>
		void BindFrameBuffer();

		/// <summary>
		/// Unbinds the FBO so that other rendering objects can be used.
		/// </summary>
		void UnbindFrameBuffer();

		/// <summary>
		/// Updates the FBO size if {@code fitToScreen}.
		/// </summary>
		void UpdateSize();

		/// <summary>
		/// Renders the colour buffer to the display.
		/// </summary>
		void BlitToScreen();

		DepthBuffer GetDepthBufferType() const { return m_depthBufferType; }

		/// <summary>
		/// Gets the number of attachments in this FBO.
		/// </summary>
		/// <returns> The number of attachments in this FBO. </returns>
		int GetAttachments() const { return m_attachments; }

		/// <summary>
		/// Gets the number of antialiasing samples.
		/// </summary>
		/// <returns> The number of antialiasing samples. </returns>
		int GetSamples() const { return m_samples; }

		/// <summary>
		/// Sets the number antialiasing samples, and recreates the FBO.
		/// </summary>
		/// <param name="samples"> The number of antialiasing samples. </param>
		void SetSamples(const int samples);

		float GetSizeScalar() const { return m_sizeScalar; }

		void SetSizeScalar(const float &sizeScalar);

		void SetSize(const int &width, const int &height);

#if 0
		GLuint GetFrameBuffer() const { return m_frameBuffer; }

		GLuint GetColourTexture(const int &readBuffer) { return m_colourTexture[readBuffer]; }

		/// <summary>
		/// Gets the depth texture.
		/// </summary>
		/// <returns> The OpenGL depth texture id. </returns>
		GLuint GetDepthTexture() const { return m_depthTexture; }

		GLuint SetDepthTexture(const int &depthTexture) { m_depthTexture = depthTexture; }

		GLuint GetDepthBuffer() const { return m_depthBuffer; }

		GLuint *GetColourBuffer() const { return m_colourBuffer; }
#endif

		/// <summary>
		/// Blits this FBO and all attachments to another FBO.
		/// </summary>
		/// <param name="source"> The source fbo </param>
		/// <param name="output"> The other FBO to blit to. </param>
		static void ResolveFBO(Fbo *source, Fbo *output);

		/// <summary>
		/// Blits this FBO attachment to another FBO attachment.
		/// </summary>
		/// <param name="source"> The source fbo </param>
		/// <param name="readBuffer"> The colour attachment to be read from. </param>
		/// <param name="drawBuffer"> The colour draw buffer to be written to. </param>
		/// <param name="output"> The other FBO to blit to. </param>
		static void ResolveFBO(Fbo *source, const int readBuffer, const int drawBuffer, Fbo *output);
	private:
		/// <summary>
		/// Initializes the FBO.
		/// </summary>
		void Initialize();

		void DetermineDrawBuffers();

		void LimitFBOSize();

		void CreateTextureAttachment(const int attachment);

		void CreateDepthBufferAttachment();

		void CreateDepthTextureAttachment();

		void AttachMultisampleColourBuffer(const int attachment);

		void Clear();
	};
}
