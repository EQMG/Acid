set(_acid_graphics_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/Buffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/InstanceBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/PushHandler.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/StorageBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/StorageHandler.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/UniformBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/UniformHandler.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Commands/CommandBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Commands/CommandPool.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Descriptors/Descriptor.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Descriptors/DescriptorSet.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Descriptors/DescriptorsHandler.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Devices/Instance.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Devices/LogicalDevice.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Devices/PhysicalDevice.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Devices/Surface.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Graphics.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Images/Image.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Images/Image2d.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Images/Image2dArray.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Images/ImageCube.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Images/ImageDepth.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Pipelines/Pipeline.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Pipelines/PipelineCompute.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Pipelines/PipelineGraphics.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Pipelines/Shader.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Renderer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Renderpass/Framebuffers.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Renderpass/Renderpass.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Renderpass/Swapchain.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/RenderStage.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Subrender.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/SubrenderHolder.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Gltf/GltfModel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Model.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Obj/ObjModel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/CubeModel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/CylinderModel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/DiskModel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/PatternMesh.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/RectangleModel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/SimpleMesh.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/SphereModel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Vertex2d.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Vertex3d.hpp
		#${CMAKE_CURRENT_SOURCE_DIR}/Post/Deferred/DeferredSubrender.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/BlitFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/BlurFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/CrtFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/DarkenFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/DefaultFilter.hpp
		#${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/DofFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/EmbossFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/FxaaFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/GrainFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/GreyFilter.hpp
		#${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/LensflareFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/NegativeFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/PixelFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/SepiaFilter.hpp
		#${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/SsaoFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/TiltshiftFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/ToneFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/VignetteFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/WobbleFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Pipelines/BlurPipeline.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/PostFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/PostPipeline.hpp
		)
set(_acid_graphics_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/Buffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/InstanceBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/PushHandler.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/StorageBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/StorageHandler.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/UniformBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Buffers/UniformHandler.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Commands/CommandBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Commands/CommandPool.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Descriptors/DescriptorSet.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Descriptors/DescriptorsHandler.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Devices/Instance.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Devices/LogicalDevice.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Devices/PhysicalDevice.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Devices/Surface.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Graphics.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Images/Image.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Images/Image2d.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Images/Image2dArray.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Images/ImageCube.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Images/ImageDepth.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Pipelines/PipelineCompute.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Pipelines/PipelineGraphics.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Pipelines/Shader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Renderpass/Framebuffers.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Renderpass/Renderpass.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Renderpass/Swapchain.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/RenderStage.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Graphics/SubrenderHolder.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Gltf/GltfModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Model.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Obj/ObjModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/CubeModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/CylinderModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/DiskModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/PatternMesh.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/RectangleModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/SimpleMesh.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Models/Shapes/SphereModel.cpp
		#${CMAKE_CURRENT_SOURCE_DIR}/Post/Deferred/DeferredSubrender.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/BlitFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/BlurFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/CrtFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/DarkenFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/DefaultFilter.cpp
		#${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/DofFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/EmbossFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/FxaaFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/GrainFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/GreyFilter.cpp
		#${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/LensflareFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/NegativeFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/PixelFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/SepiaFilter.cpp
		#${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/SsaoFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/TiltshiftFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/ToneFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/VignetteFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/WobbleFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Pipelines/BlurPipeline.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/PostFilter.cpp
		)
set(_acid_graphics_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/GraphicsExport.hpp
		)
set(_acid_graphics_third_party_headers
		${ACID_THIRD_PARTY_DIR}/tinygltf/tiny_gltf.h
		${ACID_THIRD_PARTY_DIR}/tinyobj/tiny_obj.h
		)
set(_acid_graphics_third_party_sources
		${ACID_THIRD_PARTY_DIR}/tinygltf/tiny_gltf.cpp
		${ACID_THIRD_PARTY_DIR}/tinyobj/tiny_obj.cpp
		)

add_library(Acid.Graphics
        ${_acid_graphics_headers}
        ${_acid_graphics_sources}
		${_acid_graphics_generated_headers}
        ${_acid_graphics_third_party_headers}
        ${_acid_graphics_third_party_sources}
        )

include(GenerateExportHeader)
generate_export_header(Acid.Graphics EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/GraphicsExport.hpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_graphics_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_graphics_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_acid_graphics_generated_headers})
source_group(TREE "${ACID_THIRD_PARTY_DIR}" PREFIX "ThirdParty" FILES ${_acid_graphics_third_party_headers})
source_group(TREE "${ACID_THIRD_PARTY_DIR}" PREFIX "ThirdParty" FILES ${_acid_graphics_third_party_sources})

# TODO: Prebuilt glslang binaries are unreliable between systems
#find_package(glslang 11.6.0 QUIET)
if(NOT glslang_FOUND)
	set(ENABLE_SPVREMAPPER OFF CACHE INTERNAL "Enables building of SPVRemapper")
	set(ENABLE_GLSLANG_BINARIES OFF CACHE INTERNAL "Builds glslangValidator and spirv-remap")
	set(ENABLE_HLSL OFF CACHE INTERNAL "Enables HLSL input support")
	set(ENABLE_RTTI ON CACHE INTERNAL "Enables RTTI")
	set(ENABLE_EXCEPTIONS ON CACHE INTERNAL "Enables Exceptions")
	set(ENABLE_CTEST OFF CACHE INTERNAL "Enables testing")
	set(SKIP_GLSLANG_INSTALL ON CACHE INTERNAL "Skip installation")
	set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
	set(BUILD_SHARED_LIBS OFF)
	add_subdirectory(${ACID_THIRD_PARTY_DIR}/glslang glslang)
	set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")

	set_target_properties(glslang PROPERTIES FOLDER "External/glslang")
	#set_target_properties(glslang-build-info PROPERTIES FOLDER "External/glslang")
	set_target_properties(GenericCodeGen PROPERTIES FOLDER "External/glslang")
	set_target_properties(MachineIndependent PROPERTIES FOLDER "External/glslang")
	set_target_properties(OGLCompiler PROPERTIES FOLDER "External/glslang")
	set_target_properties(OSDependent PROPERTIES FOLDER "External/glslang")
	set_target_properties(SPIRV PROPERTIES FOLDER "External/glslang")

	set(GLSLANG_LIBRARIES glslang SPIRV)
else()
	set(GLSLANG_INCLUDE_DIRS "${GLSLANG_INCLUDE_DIR}" "${SPIRV_INCLUDE_DIR}")
	set(GLSLANG_LIBRARIES glslang::glslang glslang::SPIRV)
endif()

set_target_properties(Acid.Graphics PROPERTIES FOLDER "Acid")
target_compile_features(Acid.Graphics PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Graphics
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/tinygltf>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/tinyobj>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Graphics>
		PRIVATE
		$<$<BOOL:${GLSLANG_INCLUDE_DIRS}>:${GLSLANG_INCLUDE_DIRS}>
		)
target_link_libraries(Acid.Graphics
		PUBLIC
		Acid.Core
		Acid.Context
		PRIVATE
		${GLSLANG_LIBRARIES}
		)

install(TARGETS Acid.Graphics
		EXPORT AcidGraphicsExport
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
install(EXPORT AcidGraphicsExport
		FILE AcidGraphicsConfig.cmake
		#NAMESPACE acid::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
install_headers_with_dir("Graphics" "${CMAKE_CURRENT_SOURCE_DIR}" "${_acid_graphics_headers}")
install_headers_with_dir("Graphics" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_acid_graphics_generated_headers}")
install_headers_with_dir("third_party" "${ACID_THIRD_PARTY_DIR}" "${_acid_graphics_third_party_headers}")
