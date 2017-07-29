#pragma once

#include "../framework/framework.h"
#include "../shaders/shader.h"
#include "../textures/texture.h"

#ifdef FLOUNDER_PLATFORM_WEB
#include <emscripten/emscripten.h>
#include <GLFW/glfw3.h>
#else
#include <GL/glew.h>
#endif

#include "imanagerrender.h"

namespace flounder
{
	class renderer :
		public imodule
	{
	private:
		imanagerrender *m_managerRender;

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

		/// <summary>
		/// Creates a new renderer module.
		/// </summary>
		renderer();

		/// <summary>
		/// Deconstructor for the renderer module.
		/// </summary>
		~renderer();

		void update() override;

		/// <summary>
		/// Gets the renderer manager.
		/// </summary>
		/// <returns> The renderer manager. </returns>
		inline imanagerrender *getManager() { return m_managerRender; }

		/// <summary>
		/// Sets the current renderer manager to a new renderer manager.
		/// </summary>
		/// <param name="rendererMaster"> The new renderer manager. </param>
		inline void setManager(imanagerrender *managerRender) { m_managerRender = managerRender; }

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

		/// <summary>
		/// Sets if the objects rendered will be added to the depth mask.
		/// </summary>
		/// <param name="depthMask"> Should the depth mask be enabled. </param>
		void depthMask(const bool &depthMask);

		/// <summary>
		/// Gets if the display currently in wireframe mode.
		/// </summary>
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

		/// <summary>
		/// Enables OpenGL for the scissor test.
		/// </summary>
		/// <param name="x"> The x position in pixels. </param>
		/// <param name="y"> The y position in pixels. </param>
		/// <param name="width"> The width in pixels. </param>
		/// <param name="height"> The height in pixels. </param>
		void scissorEnable(const int &x, const int &y, const int &width, const int &height);

		/// <summary>
		/// Disables the scissor test.
		/// </summary>
		void scissorDisable();

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
