#pragma once

#include <string>

#include "../devices/display.h"

#include "depthbuffer.h"

namespace flounder {
	/// <summary>
	/// A builder used to set fbo parameters for loading.
	/// </summary>
	class fbobuilder
	{
	public:
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

		/// <summary>
		/// Creates a new fbo builder.
		/// </summary>
		/// <param name="width"> The FBO's initial width. </param>
		/// <param name="height"> The FBO's initial height. </param>
		fbobuilder(const int width, const int height);

		/// <summary>
		/// Creates a new fbo builder.
		/// </summary>
		fbobuilder();

		/// <summary>
		/// Deconstructor for the shader builder.
		/// </summary>
		~fbobuilder();

		/// <summary>
		/// Sets the type of depth buffer to use.
		/// </summary>
		/// <param name="type"> The depth buffer to use.
		/// </param>
		/// <returns> this. </returns>
		fbobuilder *depthBuffer(const depthbuffer type);

		/// <summary>
		/// Disables the colour buffer.
		/// </summary>
		/// <returns> this. </returns>
		fbobuilder *noColourBuffer();

		/// <summary>
		/// Sets the texture to not use linear filtering.
		/// </summary>
		/// <returns> this. </returns>
		fbobuilder *nearestFiltering();

		/// <summary>
		/// Sets if the textures will even be bothered with wrapping.
		/// </summary>
		/// <returns> this. </returns>
		fbobuilder *disableTextureWrap();

		/// <summary>
		/// Sets the texture to repeat.
		/// </summary>
		/// <returns> this. </returns>
		fbobuilder *repeatTexture();

		/// <summary>
		/// Enables / disables the alpha channel.
		/// </summary>
		/// <param name="alpha"> If the alpha channel will be enabled.
		/// </param>
		/// <returns> this. </returns>
		fbobuilder *withAlphaChannel(const bool alpha);

		/// <summary>
		/// Sets antialiased to true and adds samples.
		/// </summary>
		/// <param name="samples"> How many MFAA samples should be used on the FBO. Zero disables multisampling.
		/// </param>
		/// <returns> this. </returns>
		fbobuilder *antialias(const int samples);

		/// <summary>
		/// Sets the amount of colour attachments to create.
		/// </summary>
		/// <param name="attachments"> The amount of attachments to create.
		/// </param>
		/// <returns> this. </returns>
		fbobuilder *attachments(const int attachments);

		/// <summary>
		/// Sets if the FBO will be fit to the screen.
		/// </summary>
		/// <param name="sizeScalar"> A scalar factor between the FBO and the screen, enabled when {@code fitToScreen} is enabled. (1.0f disables scalar).
		/// </param>
		/// <returns> this. </returns>
		fbobuilder *fitToScreen(const float sizeScalar);
	};
}
