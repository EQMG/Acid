#pragma once

//
// Flounder header file
//

#include "camera/camera.h"
#include "camera/icamera.h"
#include "camera/iplayer.h"

#include "deferred/deferredrenderer.h"

#include "devices/audio.h"
#include "devices/display.h"
#include "devices/joysticks.h"
#include "devices/keyboard.h"
#include "devices/mouse.h"

#include "entities/entity.h"

#include "events/eventchange.h"
#include "events/events.h"
#include "events/eventstandard.h"
#include "events/eventtime.h"
#include "events/ievent.h"

#include "fbos/depthbuffer.h"
#include "fbos/fbo.h"

#include "framework/framework.h"
#include "framework/glfw/glfwupdater.h"
#include "framework/imodule.h"
#include "framework/iupdater.h"

#include "helpers/helperarray.h"
#include "helpers/helperfile.h"
#include "helpers/helperstring.h"

#include "inputs/axiscompound.h"
#include "inputs/axisjoystick.h"
#include "inputs/buttoncompound.h"
#include "inputs/buttonjoystick.h"
#include "inputs/buttonkeyboard.h"
#include "inputs/buttonmouse.h"
#include "inputs/iaxis.h"
#include "inputs/ibutton.h"

#include "lights/attenuation.h"
#include "lights/fog.h"
#include "lights/light.h"

#include "loaders/loaders.h"

#include "maths/colour.h"
#include "maths/delta.h"
#include "maths/maths.h"
#include "maths/matrix2x2.h"
#include "maths/matrix3x3.h"
#include "maths/matrix4x4.h"
#include "maths/quaternion.h"
#include "maths/timer.h"
#include "maths/vector2.h"
#include "maths/vector3.h"
#include "maths/vector4.h"

#include "models/model.h"
#include "models/vertexdata.h"

#include "noise/noiseclassic.h"
#include "noise/noiseperlin.h"
#include "noise/noisesimplex.h"

#include "physics/aabb.h"
#include "physics/collider.h"
#include "physics/frustum.h"
#include "physics/intersect.h"
#include "physics/ray.h"
#include "physics/sphere.h"

#include "post/filters/filterbloom1.h"
#include "post/filters/filterbloom2.h"
#include "post/filters/filterblurhorizontal.h"
#include "post/filters/filterblurvertical.h"
#include "post/filters/filtercombine.h"
#include "post/filters/filtercrt.h"
#include "post/filters/filterdarken.h"
#include "post/filters/filteremboss.h"
#include "post/filters/filterfxaa.h"
#include "post/filters/filtergrain.h"
#include "post/filters/filtergrey.h"
#include "post/filters/filterlensflare.h"
#include "post/filters/filtermotion.h"
#include "post/filters/filternegative.h"
#include "post/filters/filterpixel.h"
#include "post/filters/filtersepia.h"
#include "post/filters/filtertiltshift.h"
#include "post/filters/filtertone.h"
#include "post/filters/filterwobble.h"
#include "post/pipelines/pipelinebloom.h"
#include "post/pipelines/pipelinegaussian.h"
#include "post/pipelines/pipelinepaused.h"
#include "post/ipostfilter.h"
#include "post/ipostpipeline.h"

#include "processing/iprocessor.h"
#include "processing/irequest.h"
#include "processing/opengl/processoropengl.h"
#include "processing/opengl/requestopengl.h"
#include "processing/processing.h"
#include "processing/queue.h"
#include "processing/resource/processorresource.h"
#include "processing/resource/requestresource.h"

#include "renderer/irenderer.h"
#include "renderer/irenderermaster.h"
#include "renderer/renderer.h"

#include "shaders/shader.h"
#include "shaders/shadertype.h"

#include "skybox/rendererskybox.h"
#include "skybox/skybox.h"

#include "standards/istandard.h"
#include "standards/standards.h"

#include "tasks/tasks.h"

#include "textures/stb_image.h"
#include "textures/texture.h"

#include "visual/driverbounce.h"
#include "visual/driverconstant.h"
#include "visual/driverfade.h"
#include "visual/driverlinear.h"
#include "visual/driversinwave.h"
#include "visual/driverslide.h"
#include "visual/idriver.h"
#include "visual/interpolation/smoothfloat.h"
