#include "SubrenderFonts2.hpp"

#include "FontType.hpp"
#include "Uis/Uis.hpp"
#include "Text.hpp"

namespace acid
{
SubrenderFonts2::SubrenderFonts2(const Pipeline::Stage &pipelineStage) :
	Subrender(pipelineStage),
	m_pipeline(pipelineStage, {"Shaders/Fonts2/Font.vert", "Shaders/Fonts2/Font.frag"}, {FontType::Instance::GetVertexInput()}, {}, PipelineGraphics::Mode::Polygon,
	PipelineGraphics::Depth::None, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE)
{
}

void SubrenderFonts2::Render(const CommandBuffer &commandBuffer)
{
	std::map<std::shared_ptr<FontType>, std::vector<Text *>> sorted;

	for (const auto &screenObject : Uis::Get()->GetObjects())
	{
		if (!screenObject->IsEnabled())
		{
			continue;
		}

		auto object = dynamic_cast<Text *>(screenObject);

		if (object)
		{
			auto it = sorted.find(object->GetFontType());

			if (it == sorted.end())
			{
				sorted.emplace(object->GetFontType(), std::vector<Text *>{});
				it = sorted.find(object->GetFontType());
			}

			(*it).second.emplace_back(object);
		}
	}

	m_pipeline.BindPipeline(commandBuffer);

	for (auto &[type, typeTexts] : sorted)
	{
		type->Update(typeTexts);
		type->CmdRender(commandBuffer, m_pipeline);
	}
}
}
