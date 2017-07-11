#pragma once

#include "../framework/framework.h"
#include "../shaders/shader.h"
#include "../textures/texture.h"
#include "irenderermaster.h"

namespace flounder {
	class renderer :
		public imodule
	{
	private:
		irenderermaster *m_renderermaster;

		bool m_cullingBackFace;
		bool m_depthMask;
		bool m_inWireframe;
		bool m_isAlphaBlending;
		bool m_additiveBlending;
		bool m_antialiasing;
	public:
		static renderer *get()
		{
			return (renderer*) framework::get()->getInstance("renderer");
		}

		renderer();

		~renderer();

		void loadRendererMaster(irenderermaster *renderermaster);

		void init();

		void update();

		inline irenderermaster *getRendererMaster() { return m_renderermaster; }

		/// <summary>
		/// Prepares the screen for a new render.
		/// </summary>
		/// <param name="colour"> The clear colour. </param>
		void prepareNewRenderParse(colour *colour);

		/// <summary>
		/// Prepares the screen for a new render.
		/// </summary>
		/// <param name="r"> The r component of the clear colour. </param>
		/// <param name="g"> The g component of the clear colour. </param>
		/// <param name="b"> The b component of the clear colour. </param>
		void prepareNewRenderParse(float r, float g, float b);

		/// <summary>
		/// Toggles the culling of back-faces.
		/// </summary>
		/// <param name="cull"> Should back faces be culled. </param>
		void cullBackFaces(bool cull);

		/// <summary>
		/// Enables depth testing.
		/// </summary>
		void enableDepthTesting();

		/// <summary>
		/// Disables depth testing.
		/// </summary>
		void disableDepthTesting();

		void depthMask(bool depthMask);

		/// <returns> Is the display currently in wireframe mode. </returns>
		bool isInWireframe();

		/// <summary>
		/// Toggles the display to / from wireframe mode.
		/// </summary>
		/// <param name="goWireframe"> If the display should be in wireframe. </param>
		void goWireframe(bool goWireframe);

		/// <summary>
		/// Enables alpha blending.
		/// </summary>
		void enableAlphaBlending();

		/// <summary>
		/// Enables additive blending.
		/// </summary>
		void enableAdditiveBlending();

		/// <summary>
		/// Disables alpha and additive blending.
		/// </summary>
		void disableBlending();

		/// <summary>
		/// Toggles antialiasing for the rendered object.
		/// </summary>
		/// <param name="enable"> Should antialias be enabled? </param>
		void antialias(bool enable);

		/// <summary>
		/// Binds the VAO and all attributes.
		/// </summary>
		/// <param name="vaoID"> The VAO to bind. </param>
		/// <param name="n_args"> The number of attributes to enable. </param>
		/// <param name="..."> Attributes to enable. </param>
		void bindVAO(int vaoID, int n_args, ...);

		/// <summary>
		/// Unbinds the current VAO and all attributes.
		/// </summary>
		/// <param name="n_args"> The number of attributes to disable. </param>
		/// <param name="..."> Attributes to disable. </param>
		void unbindVAO(int n_args, ...);

		void scissor(int x, int y, int width, int height);

		/// <summary>
		/// Binds a OpenGL texture to a blank ID.
		/// </summary>
		/// <param name="texture"> The texture to bind. </param>
		/// <param name="bankID"> The shaders blank ID to bind to. </param>
		void bindTexture(texture *texture, int bankID);

		/// <summary>
		/// Binds a OpenGL texture to a blank ID.
		/// </summary>
		/// <param name="textureID"> The texture to bind. </param>
		/// <param name="glTarget"> The OpenGL texture type to bind to. {@code GL_TEXTURE_CUBE_MAP}, and {@code GL_TEXTURE_2D} are the most common types. </param>
		/// <param name="bankID"> The shaders blank ID to bind to. </param>
		void bindTexture(int textureID, int glTarget, int bankID);

		/// <summary>
		/// Binds the OpenGL texture to a blank ID.
		/// </summary>
		/// <param name="textureID"> The texture to bind. </param>
		/// <param name="lodBias"> The LOD to load to texture at. </param>
		/// <param name="bankID"> The shaders blank ID to bind to. </param>
		void bindTextureLOD(int textureID, int lodBias, int bankID);

		/// <summary>
		/// Renders a bound model on a enabled shader using glDrawArrays.
		/// </summary>
		/// <param name="glMode"> The OpenGL mode to draw in. </param>
		/// <param name="glLength"> The length of the model. </param>
		void renderArrays(GLenum glMode, GLsizei glLength);

		/// <summary>
		/// Renders a bound model on a enabled shader using glDrawElements.
		/// </summary>
		/// <param name="glMode"> The OpenGL mode to draw in. </param>
		/// <param name="glType"> The OpenGL type to draw in. </param>
		/// <param name="glLength"> The length of the model. </param>
		void renderElements(GLenum glMode, GLenum glType, GLsizei glLength);

		/// <summary>
		/// Renders a bound model on a enabled shader using glDrawArraysInstancedARB.
		/// </summary>
		/// <param name="glMode"> The OpenGL mode to draw in. </param>
		/// <param name="glLength"> The length of the model. </param>
		/// <param name="glPrimCount"> How many primitives rendered. </param>
		void renderInstanced(GLenum glMode, GLsizei glLength, GLsizei glPrimCount);
	};
}
