set(FLOUNDERENGINE_HEADERS_
	"src/deferred/RendererDeferred.hpp"
	"src/deferred/UbosDeferred.hpp"
	"src/entities/components/ComponentMaterial.hpp"
	"src/entities/components/ComponentGlow.hpp"
	"src/entities/components/ComponentSun.hpp"
	"src/entities/components/ComponentLight.hpp"
	"src/entities/components/ComponentCollision.hpp"
	"src/entities/components/ComponentCollider.hpp"
	"src/entities/components/ComponentSway.hpp"
	"src/entities/components/ComponentModel.hpp"
	"src/entities/EntityRender.hpp"
	"src/entities/Entities.hpp"
	"src/entities/UbosEntities.hpp"
	"src/fonts/FontFamily.hpp"
	"src/fonts/UbosFonts.hpp"
	"src/guis/UbosGuis.hpp"
	"src/models/MeshGenerator.hpp"
	"src/post/pipelines/PipelineBloom.hpp"
	"src/post/pipelines/PipelineGaussian.hpp"
	"src/post/pipelines/PipelinePaused.hpp"
	"src/renderer/buffers/UniformBuffer.hpp"
	"src/renderer/pipelines/PipelineCreate.hpp"
	"src/renderer/stencils/DepthStencil.hpp"
	"src/camera/Camera.hpp"
	"src/camera/ICamera.hpp"
	"src/camera/IPlayer.hpp"
	"src/devices/Audio.hpp"
	"src/devices/Display.hpp"
	"src/devices/Joysticks.hpp"
	"src/devices/Keyboard.hpp"
	"src/devices/Mouse.hpp"
	"src/engine/Engine.hpp"
	"src/engine/IModule.hpp"
	"src/engine/IUpdater.hpp"
	"src/engine/ModuleUpdate.hpp"
	"src/entities/Entity.hpp"
	"src/entities/IComponent.hpp"
	"src/events/EventChange.hpp"
	"src/events/Events.hpp"
	"src/events/EventStandard.hpp"
	"src/events/EventTime.hpp"
	"src/events/IEvent.hpp"
	"src/Flounder.hpp"
	"src/fonts/Character.hpp"
	"src/fonts/FontType.hpp"
	"src/fonts/Line.hpp"
	"src/fonts/Metafile.hpp"
	"src/fonts/RendererFonts.hpp"
	"src/fonts/Text.hpp"
	"src/fonts/Word.hpp"
	"src/guis/Gui.hpp"
	"src/guis/RendererGuis.hpp"
	"src/helpers/HelperArray.hpp"
	"src/helpers/HelperFile.hpp"
	"src/helpers/HelperString.hpp"
	"src/inputs/AxisButton.hpp"
	"src/inputs/AxisCompound.hpp"
	"src/inputs/AxisJoystick.hpp"
	"src/inputs/ButtonCompound.hpp"
	"src/inputs/ButtonJoystick.hpp"
	"src/inputs/ButtonKeyboard.hpp"
	"src/inputs/ButtonMouse.hpp"
	"src/inputs/IAxis.hpp"
	"src/inputs/IButton.hpp"
	"src/lights/Fog.hpp"
	"src/lights/Light.hpp"
	"src/maths/Colour.hpp"
	"src/maths/Delta.hpp"
	"src/maths/Maths.hpp"
	"src/maths/Matrix2.hpp"
	"src/maths/Matrix3.hpp"
	"src/maths/Matrix4.hpp"
	"src/maths/Quaternion.hpp"
	"src/maths/Timer.hpp"
	"src/maths/Transform.hpp"
	"src/maths/Vector2.hpp"
	"src/maths/Vector3.hpp"
	"src/maths/Vector4.hpp"
	"src/models/Model.hpp"
	"src/models/VertexData.hpp"
	"src/noise/NoiseFast.hpp"
	"src/particles/Particle.hpp"
	"src/particles/Particles.hpp"
	"src/particles/ParticleSystem.hpp"
	"src/particles/ParticleType.hpp"
	"src/particles/RendererParticles.hpp"
	"src/particles/spawns/ISpawnParticle.hpp"
	"src/particles/spawns/SpawnCircle.hpp"
	"src/particles/spawns/SpawnLine.hpp"
	"src/particles/spawns/SpawnPoint.hpp"
	"src/particles/spawns/SpawnSphere.hpp"
	"src/physics/Aabb.hpp"
	"src/physics/Frustum.hpp"
	"src/physics/ICollider.hpp"
	"src/physics/Intersect.hpp"
	"src/physics/Ray.hpp"
	"src/physics/Sphere.hpp"
	"src/platforms/PlatformUpdater.hpp"
	"src/platforms/Platform.hpp"
	"src/post/filters/FilterBloom1.hpp"
	"src/post/filters/FilterBloom2.hpp"
	"src/post/filters/FilterBlurHorizontal.hpp"
	"src/post/filters/FilterBlurVertical.hpp"
	"src/post/filters/FilterCombine.hpp"
	"src/post/filters/FilterCrt.hpp"
	"src/post/filters/FilterDarken.hpp"
	"src/post/filters/FilterDefault.hpp"
	"src/post/filters/FilterEmboss.hpp"
	"src/post/filters/FilterFxaa.hpp"
	"src/post/filters/FilterGrain.hpp"
	"src/post/filters/FilterGrey.hpp"
	"src/post/filters/FilterLensflare.hpp"
	"src/post/filters/FilterMotion.hpp"
	"src/post/filters/FilterNegative.hpp"
	"src/post/filters/FilterPixel.hpp"
	"src/post/filters/FilterSepia.hpp"
	"src/post/filters/FilterTiltshift.hpp"
	"src/post/filters/FilterTone.hpp"
	"src/post/filters/FilterWobble.hpp"
	"src/post/IPostFilter.hpp"
	"src/post/IPostPipeline.hpp"
	"src/processing/graphic/ProcessorGraphic.hpp"
	"src/processing/graphic/RequestGraphic.hpp"
	"src/processing/IProcessor.hpp"
	"src/processing/IRequest.hpp"
	"src/processing/Processing.hpp"
	"src/processing/Queue.hpp"
	"src/processing/resource/ProcessorResource.hpp"
	"src/processing/resource/RequestResource.hpp"
	"src/renderer/buffers/Buffer.hpp"
	"src/renderer/buffers/IndexBuffer.hpp"
	"src/models/Vertex.hpp"
	"src/renderer/buffers/VertexBuffer.hpp"
	"src/renderer/IManagerRender.hpp"
	"src/renderer/IRenderer.hpp"
	"src/renderer/pass/RenderPass.hpp"
	"src/renderer/pipelines/Pipeline.hpp"
	"src/renderer/queue/QueueFamily.hpp"
	"src/renderer/Renderer.hpp"
	"src/renderer/swapchain/Framebuffers.hpp"
	"src/renderer/swapchain/Swapchain.hpp"
	"src/shadows/RendererShadows.hpp"
	"src/shadows/ShadowBox.hpp"
	"src/shadows/Shadows.hpp"
	"src/shadows/UbosShadows.hpp"
	"src/skyboxes/RendererSkyboxes.hpp"
	"src/skyboxes/Skybox.hpp"
	"src/skyboxes/Skyboxes.hpp"
	"src/skyboxes/UbosSkyboxes.hpp"
	"src/sounds/Sound.hpp"
	"src/space/ISpatialObject.hpp"
	"src/space/ISpatialStructure.hpp"
	"src/space/StructureBasic.hpp"
	"src/standards/IStandard.hpp"
	"src/standards/Standards.hpp"
	"src/stb/stb_image.h"
	"src/tasks/Tasks.hpp"
	"src/terrains/RendererTerrains.hpp"
	"src/terrains/UbosTerrains.hpp"
	"src/terrains/Terrain.hpp"
	"src/terrains/Terrains.hpp"
	"src/entities/RendererEntities.hpp"
	"src/textures/Cubemap.hpp"
	"src/textures/Texture.hpp"
	"src/uis/IManagerUis.hpp"
	"src/uis/InputButton.hpp"
	"src/uis/InputDelay.hpp"
	"src/uis/InputGrabber.hpp"
	"src/uis/InputSlider.hpp"
	"src/uis/InputText.hpp"
	"src/uis/OverlayStartup.hpp"
	"src/fonts/Justify.hpp"
	"src/uis/UiBound.hpp"
	"src/uis/UiObject.hpp"
	"src/uis/Uis.hpp"
	"src/uis/UiSelector.hpp"
	"src/visual/DriverBounce.hpp"
	"src/visual/DriverConstant.hpp"
	"src/visual/DriverFade.hpp"
	"src/visual/DriverLinear.hpp"
	"src/visual/DriverSinwave.hpp"
	"src/visual/DriverSlide.hpp"
	"src/visual/IDriver.hpp"
	"src/visual/interpolation/SmoothFloat.hpp"
	"src/waters/RendererWaters.hpp"
	"src/waters/UbosWaters.hpp"
	"src/waters/Water.hpp"
	"src/waters/Waters.hpp"
	"src/worlds/Worlds.hpp"
)

set(FLOUNDERENGINE_SOURCES_
	"src/deferred/RendererDeferred.cpp"
	"src/entities/components/ComponentMaterial.cpp"
	"src/entities/components/ComponentGlow.cpp"
	"src/entities/components/ComponentSun.cpp"
	"src/entities/components/ComponentLight.cpp"
	"src/entities/components/ComponentCollision.cpp"
	"src/entities/components/ComponentCollider.cpp"
	"src/entities/components/ComponentSway.cpp"
	"src/entities/components/ComponentModel.cpp"
	"src/entities/Entities.cpp"
	"src/fonts/FontFamily.cpp"
	"src/helpers/HelperArray.cpp"
	"src/helpers/HelperFile.cpp"
	"src/helpers/HelperString.cpp"
	"src/maths/Colour.cpp"
	"src/models/MeshGenerator.cpp"
	"src/models/Vertex.cpp"
	"src/post/pipelines/PipelineBloom.cpp"
	"src/post/pipelines/PipelineGaussian.cpp"
	"src/post/pipelines/PipelinePaused.cpp"
	"src/renderer/buffers/UniformBuffer.cpp"
	"src/renderer/stencils/DepthStencil.cpp"
	"src/camera/Camera.cpp"
	"src/devices/Audio.cpp"
	"src/devices/Display.cpp"
	"src/devices/Joysticks.cpp"
	"src/devices/Keyboard.cpp"
	"src/devices/Mouse.cpp"
	"src/engine/Engine.cpp"
	"src/entities/Entity.cpp"
	"src/events/EventChange.cpp"
	"src/events/Events.cpp"
	"src/events/EventStandard.cpp"
	"src/events/EventTime.cpp"
	"src/fonts/Character.cpp"
	"src/fonts/FontType.cpp"
	"src/fonts/Line.cpp"
	"src/fonts/Metafile.cpp"
	"src/fonts/RendererFonts.cpp"
	"src/fonts/Text.cpp"
	"src/fonts/Word.cpp"
	"src/guis/Gui.cpp"
	"src/guis/RendererGuis.cpp"
	"src/inputs/AxisButton.cpp"
	"src/inputs/AxisCompound.cpp"
	"src/inputs/AxisJoystick.cpp"
	"src/inputs/ButtonCompound.cpp"
	"src/inputs/ButtonJoystick.cpp"
	"src/inputs/ButtonKeyboard.cpp"
	"src/inputs/ButtonMouse.cpp"
	"src/lights/Fog.cpp"
	"src/lights/Light.cpp"
	"src/maths/Delta.cpp"
	"src/maths/Matrix2.cpp"
	"src/maths/Matrix3.cpp"
	"src/maths/Matrix4.cpp"
	"src/maths/Quaternion.cpp"
	"src/maths/Timer.cpp"
	"src/maths/Transform.cpp"
	"src/maths/Vector2.cpp"
	"src/maths/Vector3.cpp"
	"src/maths/Vector4.cpp"
	"src/models/Model.cpp"
	"src/models/VertexData.cpp"
	"src/noise/NoiseFast.cpp"
	"src/particles/Particle.cpp"
	"src/particles/Particles.cpp"
	"src/particles/ParticleSystem.cpp"
	"src/particles/ParticleType.cpp"
	"src/particles/RendererParticles.cpp"
	"src/particles/spawns/SpawnCircle.cpp"
	"src/particles/spawns/SpawnLine.cpp"
	"src/particles/spawns/SpawnPoint.cpp"
	"src/particles/spawns/SpawnSphere.cpp"
	"src/physics/Aabb.cpp"
	"src/physics/Frustum.cpp"
	"src/physics/Ray.cpp"
	"src/physics/Sphere.cpp"
	"src/platforms/PlatformUpdater.cpp"
	"src/platforms/Platform.cpp"
	"src/post/filters/FilterBloom1.cpp"
	"src/post/filters/FilterBloom2.cpp"
	"src/post/filters/FilterBlurHorizontal.cpp"
	"src/post/filters/FilterBlurVertical.cpp"
	"src/post/filters/FilterCombine.cpp"
	"src/post/filters/FilterCrt.cpp"
	"src/post/filters/FilterDarken.cpp"
	"src/post/filters/FilterDefault.cpp"
	"src/post/filters/FilterEmboss.cpp"
	"src/post/filters/FilterFxaa.cpp"
	"src/post/filters/FilterGrain.cpp"
	"src/post/filters/FilterGrey.cpp"
	"src/post/filters/FilterLensflare.cpp"
	"src/post/filters/FilterMotion.cpp"
	"src/post/filters/FilterNegative.cpp"
	"src/post/filters/FilterPixel.cpp"
	"src/post/filters/FilterSepia.cpp"
	"src/post/filters/FilterTiltshift.cpp"
	"src/post/filters/FilterTone.cpp"
	"src/post/filters/FilterWobble.cpp"
	"src/post/IPostFilter.cpp"
	"src/post/IPostPipeline.cpp"
	"src/processing/graphic/ProcessorGraphic.cpp"
	"src/processing/Processing.cpp"
	"src/processing/resource/ProcessorResource.cpp"
	"src/renderer/buffers/Buffer.cpp"
	"src/renderer/buffers/IndexBuffer.cpp"
	"src/renderer/buffers/VertexBuffer.cpp"
	"src/renderer/pass/RenderPass.cpp"
	"src/renderer/pipelines/Pipeline.cpp"
	"src/renderer/queue/QueueFamily.cpp"
	"src/renderer/Renderer.cpp"
	"src/renderer/swapchain/Framebuffers.cpp"
	"src/renderer/swapchain/Swapchain.cpp"
	"src/shadows/RendererShadows.cpp"
	"src/shadows/ShadowBox.cpp"
	"src/shadows/Shadows.cpp"
	"src/skyboxes/RendererSkyboxes.cpp"
	"src/skyboxes/Skybox.cpp"
	"src/skyboxes/Skyboxes.cpp"
	"src/sounds/Sound.cpp"
	"src/space/StructureBasic.cpp"
	"src/standards/Standards.cpp"
	"src/stb/stb_image.c"
	"src/tasks/Tasks.cpp"
	"src/terrains/RendererTerrains.cpp"
	"src/terrains/Terrain.cpp"
	"src/terrains/Terrains.cpp"
	"src/entities/RendererEntities.cpp"
	"src/textures/Cubemap.cpp"
	"src/textures/Texture.cpp"
	"src/uis/InputButton.cpp"
	"src/uis/InputDelay.cpp"
	"src/uis/InputGrabber.cpp"
	"src/uis/InputSlider.cpp"
	"src/uis/InputText.cpp"
	"src/uis/OverlayStartup.cpp"
	"src/uis/UiBound.cpp"
	"src/uis/UiObject.cpp"
	"src/uis/Uis.cpp"
	"src/uis/UiSelector.cpp"
	"src/visual/DriverBounce.cpp"
	"src/visual/DriverConstant.cpp"
	"src/visual/DriverFade.cpp"
	"src/visual/DriverLinear.cpp"
	"src/visual/DriverSinwave.cpp"
	"src/visual/DriverSlide.cpp"
	"src/visual/interpolation/SmoothFloat.cpp"
	"src/waters/RendererWaters.cpp"
	"src/waters/Water.cpp"
	"src/waters/Waters.cpp"
	"src/worlds/Worlds.cpp"
)

file(GLOB FLOUNDERENGINE_HEADERS__
    "*.h"
    "*.hpp"
)

file(GLOB FLOUNDERENGINE_SOURCES__
    "*."
    "*.cpp"
)

message(WARNING "Headers: ${FLOUNDERENGINE_HEADERS__}")
message(WARNING "Sources: ${FLOUNDERENGINE_SOURCES__}")

source_group("Header Files" FILES ${FLOUNDERENGINE_HEADERS_})
source_group("Source Files" FILES ${FLOUNDERENGINE_SOURCES_})

set(FLOUNDERENGINE_SOURCES
	${FLOUNDERENGINE_HEADERS_}
	${FLOUNDERENGINE_SOURCES_}
)
