#include "rendererfonts.hpp"

namespace Flounder
{
	RendererFonts::RendererFonts() :
		IRenderer(),
		m_shader(new Shader("fonts", 2,
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/fonts/font.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/fonts/font.frag.spv")
		))
	{
	}

	RendererFonts::~RendererFonts()
	{
		delete m_shader;
	}

	void RendererFonts::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		PrepareRendering(clipPlane, camera);

		for (auto screenobject : *Uis::get()->GetObjects())
		{
			Text *object = dynamic_cast<Text*>(screenobject);

			if (object != nullptr)
			{
				RenderText(object);
			}
		}

		EndRendering();
	}

	void RendererFonts::PrepareRendering(const Vector4 &clipPlane, const ICamera &camera)
	{
#if 0
		// Starts the shader.
		m_shader->start();

		// Loads the uniforms.
		m_shader->loadUniform1f("aspectRatio", static_cast<float>(Display::Get()->getAspectRatio()));

		// Sets the GPU for rendering this object.
		renderer::get()->disableDepthTesting();
		renderer::get()->cullBackFaces(true);
		renderer::get()->enableAlphaBlending();
#endif
	}

	void RendererFonts::RenderText(Text *object)
	{
#if 0
		// Binds the layouts.
		renderer::get()->bindVAO(object->getModel()->getVaoID(), 2, 0, 1);
		renderer::get()->bindTexture(object->getTexture(), 0);

		// Applies the GL scissor test.
		Vector4 *scissor = object->getScissor();

		if (scissor->m_z != -1.0f && scissor->m_w != -1.0f)
		{
			renderer::get()->scissorEnable(static_cast<int>(scissor->m_x), static_cast<int>(scissor->m_y), static_cast<int>(scissor->m_z), static_cast<int>(scissor->m_w));
		}

		// Loads the uniforms.
		m_shader->loadUniform2f("size", *object->getMeshSize());
		m_shader->loadUniform4f("transform",
		                      object->getScreenPosition()->m_x, object->getScreenPosition()->m_y,
		                      object->getScreenDimensions()->m_x, object->getScreenDimensions()->m_y
		);
		m_shader->loadUniform1f("rotation", static_cast<float>(Maths::Radians(object->getRotation())));

		m_shader->loadUniform1f("alpha", object->getAlpha()); 
		m_shader->loadUniform4f("colour", object->getTextColour()->m_r, object->getTextColour()->m_g, object->getTextColour()->m_b, object->getAlpha());
		m_shader->loadUniform3f("borderColour", *object->getBorderColour());
		m_shader->loadUniform2f("edgeData", object->calculateEdgeStart(), object->calculateAntialiasSize());
		m_shader->loadUniform2f("borderSizes", object->getTotalBorderSize(), object->getGlowSize());

		// Tells the GPU to render this object.
		renderer::get()->renderArrays(GL_TRIANGLES, object->getModel()->getVaoLength());
		renderer::get()->scissorDisable();

		// Unbinds the layouts.
		renderer::get()->unbindVAO(2, 0, 1);
#endif
	}

	void RendererFonts::EndRendering()
	{
#if 0
		// Stops the shader.
		m_shader->stop();
#endif
	}
}
