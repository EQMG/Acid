#pragma once

#include "../framework/framework.h"
#include "../shaders/shader.h"
#include "../textures/texture.h"

#include "irenderermaster.h"

namespace flounder
{
	class renderer :
		public imodule
	{
	private:
		irenderermaster *m_rendererMaster;

		bool m_cullingBackFace;
		bool m_depthMask;
		bool m_inWireframe;
		bool m_isAlphaBlending;
		bool m_additiveBlending;
		bool m_antialiasing;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static renderer *get()
		{
			return static_cast<renderer*>(framework::get()->getInstance("renderer"));
		}

		renderer();

		~renderer();

		void load(irenderermaster *rendererMaster);

		void init() override;

		void update() override;

		inline irenderermaster *getRendererMaster() { return m_rendererMaster; }

		inline void setRendererMaster(irenderermaster *rendererMaster) { m_rendererMaster = rendererMaster; }

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
		/// <param name="a"> The a component of the clear colour. </param>
		void prepareNewRenderParse(const float &r, const float &g, const float &b, const float &a);

		/// <summary>
		/// Toggles the culling of back-faces.
		/// </summary>
		/// <param name="cull"> Should back faces be culled. </param>
		void cullBackFaces(const bool &cull);

		/// <summary>
		/// Enables depth testing.
		/// </summary>
		void enableDepthTesting();

		/// <summary>
		/// Disables depth testing.
		/// </summary>
		void disableDepthTesting();

		void depthMask(const bool &depthMask);

		/// <returns> Is the display currently in wireframe mode. </returns>
		bool isInWireframe();

		/// <summary>
		/// Toggles the display to / from wireframe mode.
		/// </summary>
		/// <param name="goWireframe"> If the display should be in wireframe. </param>
		void goWireframe(const bool &goWireframe);

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
		void antialias(const bool &enable);

		/// <summary>
		/// Binds the VAO and all attributes.
		/// </summary>
		/// <param name="vaoID"> The VAO to bind. </param>
		/// <param name="n_args"> The number of attributes to enable. </param>
		/// <param name="..."> Attributes to enable. </param>
		void bindVAO(const GLint &vaoID, const int n_args, ...);

		/// <summary>
		/// Unbinds the current VAO and all attributes.
		/// </summary>
		/// <param name="n_args"> The number of attributes to disable. </param>
		/// <param name="..."> Attributes to disable. </param>
		void unbindVAO(const int n_args, ...);

		void scissor(const int &x, const int &y, const int &width, const int &height);

		/// <summary>
		/// Binds a OpenGL texture to a blank ID.
		/// </summary>
		/// <param name="texture"> The texture to bind. </param>
		/// <param name="bankID"> The shaders blank ID to bind to. </param>
		void bindTexture(texture *texture, const int &bankID);

		/// <summary>
		/// Binds a OpenGL texture to a blank ID.
		/// </summary>
		/// <param name="textureID"> The texture to bind. </param>
		/// <param name="glTarget"> The OpenGL texture type to bind to. {@code GL_TEXTURE_CUBE_MAP}, and {@code GL_TEXTURE_2D} are the most common types. </param>
		/// <param name="bankID"> The shaders blank ID to bind to. </param>
		void bindTexture(const GLint &textureID, const GLenum &glTarget, const int &bankID);

		/// <summary>
		/// Binds the OpenGL texture to a blank ID.
		/// </summary>
		/// <param name="textureID"> The texture to bind. </param>
		/// <param name="lodBias"> The LOD to load to texture at. </param>
		/// <param name="bankID"> The shaders blank ID to bind to. </param>
		void bindTextureLOD(const GLint &textureID, const GLint &lodBias, const int &bankID);

		/// <summary>
		/// Renders a bound model on a enabled shader using glDrawArrays.
		/// </summary>
		/// <param name="glMode"> The OpenGL mode to draw in. </param>
		/// <param name="glLength"> The length of the model. </param>
		void renderArrays(const GLenum &glMode, const GLsizei &glLength);

		/// <summary>
		/// Renders a bound model on a enabled shader using glDrawElements.
		/// </summary>
		/// <param name="glMode"> The OpenGL mode to draw in. </param>
		/// <param name="glType"> The OpenGL type to draw in. </param>
		/// <param name="glLength"> The length of the model. </param>
		void renderElements(const GLenum &glMode, const GLenum &glType, const GLsizei &glLength);

		/// <summary>
		/// Renders a bound model on a enabled shader using glDrawArraysInstancedARB.
		/// </summary>
		/// <param name="glMode"> The OpenGL mode to draw in. </param>
		/// <param name="glLength"> The length of the model. </param>
		/// <param name="glPrimCount"> How many primitives rendered. </param>
		void renderInstanced(const GLenum &glMode, const GLsizei &glLength, const GLsizei &glPrimCount);
	};
}
