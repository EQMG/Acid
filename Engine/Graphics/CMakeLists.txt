set(_acid_graphics_headers
		Graphics/Buffers/Buffer.hpp
		Graphics/Buffers/InstanceBuffer.hpp
		Graphics/Buffers/PushHandler.hpp
		Graphics/Buffers/StorageBuffer.hpp
		Graphics/Buffers/StorageHandler.hpp
		Graphics/Buffers/UniformBuffer.hpp
		Graphics/Buffers/UniformHandler.hpp
		Graphics/Commands/CommandBuffer.hpp
		Graphics/Commands/CommandPool.hpp
		Graphics/Descriptors/Descriptor.hpp
		Graphics/Descriptors/DescriptorSet.hpp
		Graphics/Descriptors/DescriptorsHandler.hpp
		Graphics/Devices/Instance.hpp
		Graphics/Devices/LogicalDevice.hpp
		Graphics/Devices/PhysicalDevice.hpp
		Graphics/Devices/Surface.hpp
		Graphics/Graphics.hpp
		Graphics/Images/Image.hpp
		Graphics/Images/Image2d.hpp
		Graphics/Images/Image2dArray.hpp
		Graphics/Images/ImageCube.hpp
		Graphics/Images/ImageDepth.hpp
		Graphics/Pipelines/Pipeline.hpp
		Graphics/Pipelines/PipelineCompute.hpp
		Graphics/Pipelines/PipelineGraphics.hpp
		Graphics/Pipelines/Shader.hpp
		Graphics/Renderer.hpp
		Graphics/Renderpass/Framebuffers.hpp
		Graphics/Renderpass/Renderpass.hpp
		Graphics/Renderpass/Swapchain.hpp
		Graphics/RenderStage.hpp
		Graphics/Subrender.hpp
		Graphics/SubrenderHolder.hpp
		Models/Gltf/GltfModel.hpp
		Models/Model.hpp
		Models/Obj/ObjModel.hpp
		Models/Shapes/CubeModel.hpp
		Models/Shapes/CylinderModel.hpp
		Models/Shapes/DiskModel.hpp
		Models/Shapes/PatternMesh.hpp
		Models/Shapes/RectangleModel.hpp
		Models/Shapes/SimpleMesh.hpp
		Models/Shapes/SphereModel.hpp
		Models/Vertex2d.hpp
		Models/Vertex3d.hpp
		#Post/Deferred/DeferredSubrender.hpp
		Post/Filters/BlitFilter.hpp
		Post/Filters/BlurFilter.hpp
		Post/Filters/CrtFilter.hpp
		Post/Filters/DarkenFilter.hpp
		Post/Filters/DefaultFilter.hpp
		#Post/Filters/DofFilter.hpp
		Post/Filters/EmbossFilter.hpp
		Post/Filters/FxaaFilter.hpp
		Post/Filters/GrainFilter.hpp
		Post/Filters/GreyFilter.hpp
		#Post/Filters/LensflareFilter.hpp
		Post/Filters/NegativeFilter.hpp
		Post/Filters/PixelFilter.hpp
		Post/Filters/SepiaFilter.hpp
		#Post/Filters/SsaoFilter.hpp
		Post/Filters/TiltshiftFilter.hpp
		Post/Filters/ToneFilter.hpp
		Post/Filters/VignetteFilter.hpp
		Post/Filters/WobbleFilter.hpp
		Post/Pipelines/BlurPipeline.hpp
		Post/PostFilter.hpp
		Post/PostPipeline.hpp
		)
set(_acid_graphics_sources
		Graphics/Buffers/Buffer.cpp
		Graphics/Buffers/InstanceBuffer.cpp
		Graphics/Buffers/PushHandler.cpp
		Graphics/Buffers/StorageBuffer.cpp
		Graphics/Buffers/StorageHandler.cpp
		Graphics/Buffers/UniformBuffer.cpp
		Graphics/Buffers/UniformHandler.cpp
		Graphics/Commands/CommandBuffer.cpp
		Graphics/Commands/CommandPool.cpp
		Graphics/Descriptors/DescriptorSet.cpp
		Graphics/Descriptors/DescriptorsHandler.cpp
		Graphics/Devices/Instance.cpp
		Graphics/Devices/LogicalDevice.cpp
		Graphics/Devices/PhysicalDevice.cpp
		Graphics/Devices/Surface.cpp
		Graphics/Graphics.cpp
		Graphics/Images/Image.cpp
		Graphics/Images/Image2d.cpp
		Graphics/Images/Image2dArray.cpp
		Graphics/Images/ImageCube.cpp
		Graphics/Images/ImageDepth.cpp
		Graphics/Pipelines/PipelineCompute.cpp
		Graphics/Pipelines/PipelineGraphics.cpp
		Graphics/Pipelines/Shader.cpp
		Graphics/Renderpass/Framebuffers.cpp
		Graphics/Renderpass/Renderpass.cpp
		Graphics/Renderpass/Swapchain.cpp
		Graphics/RenderStage.cpp
		Graphics/SubrenderHolder.cpp
		Models/Gltf/GltfModel.cpp
		Models/Model.cpp
		Models/Obj/ObjModel.cpp
		Models/Shapes/CubeModel.cpp
		Models/Shapes/CylinderModel.cpp
		Models/Shapes/DiskModel.cpp
		Models/Shapes/PatternMesh.cpp
		Models/Shapes/RectangleModel.cpp
		Models/Shapes/SimpleMesh.cpp
		Models/Shapes/SphereModel.cpp
		#Post/Deferred/DeferredSubrender.cpp
		Post/Filters/BlitFilter.cpp
		Post/Filters/BlurFilter.cpp
		Post/Filters/CrtFilter.cpp
		Post/Filters/DarkenFilter.cpp
		Post/Filters/DefaultFilter.cpp
		#Post/Filters/DofFilter.cpp
		Post/Filters/EmbossFilter.cpp
		Post/Filters/FxaaFilter.cpp
		Post/Filters/GrainFilter.cpp
		Post/Filters/GreyFilter.cpp
		#Post/Filters/LensflareFilter.cpp
		Post/Filters/NegativeFilter.cpp
		Post/Filters/PixelFilter.cpp
		Post/Filters/SepiaFilter.cpp
		#Post/Filters/SsaoFilter.cpp
		Post/Filters/TiltshiftFilter.cpp
		Post/Filters/ToneFilter.cpp
		Post/Filters/VignetteFilter.cpp
		Post/Filters/WobbleFilter.cpp
		Post/Pipelines/BlurPipeline.cpp
		Post/PostFilter.cpp
		)

set(_acid_graphics_third_party_headers
		../../External/tinygltf/tiny_gltf.h
		../../External/tinyobj/tiny_obj.h
		)
set(_acid_graphics_third_party_sources
		../../External/tinygltf/tiny_gltf.cpp
		../../External/tinyobj/tiny_obj.cpp
		)
set(_acid_graphics_third_party_includes
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/tinygltf
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/tinyobj
		)

add_library(Acid.Graphics
        ${_acid_graphics_headers}
        ${_acid_graphics_sources}
        ${_acid_graphics_third_party_headers}
        ${_acid_graphics_third_party_sources}
        )

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Graphics EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Graphics/Export.hpp")

# TODO: Prebuilt glslang binaries are unreliable between systems
#find_package(glslang 11.6.0 QUIET)
if(NOT glslang_FOUND)
	# On MSVC shared mode must be disabled with glslang currently
	set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
	if(MSVC)
		set(BUILD_SHARED_LIBS OFF)
	endif()
	set(ENABLE_SPVREMAPPER OFF CACHE INTERNAL "Enables building of SPVRemapper")
	set(ENABLE_GLSLANG_BINARIES OFF CACHE INTERNAL "Builds glslangValidator and spirv-remap")
	set(ENABLE_HLSL OFF CACHE INTERNAL "Enables HLSL input support")
	set(ENABLE_RTTI ON CACHE INTERNAL "Enables RTTI")
	set(ENABLE_EXCEPTIONS ON CACHE INTERNAL "Enables Exceptions")
	set(ENABLE_CTEST OFF CACHE INTERNAL "Enables testing")
	set(SKIP_GLSLANG_INSTALL ON CACHE INTERNAL "Skip installation")
	add_subdirectory(../../External/glslang glslang)
	# Reset back to value before MSVC fix
	set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")

	set_target_properties(glslang PROPERTIES FOLDER "External/glslang")
	#set_target_properties(glslang-build-info PROPERTIES FOLDER "External/glslang")
	set_target_properties(GenericCodeGen PROPERTIES FOLDER "External/glslang")
	set_target_properties(MachineIndependent PROPERTIES FOLDER "External/glslang")
	set_target_properties(OGLCompiler PROPERTIES FOLDER "External/glslang")
	set_target_properties(OSDependent PROPERTIES FOLDER "External/glslang")
	set_target_properties(SPIRV PROPERTIES FOLDER "External/glslang")

	# Used in target_link_libraries()
	set(GLSLANG_LIBRARIES glslang SPIRV)
else()
	set(GLSLANG_INCLUDE_DIRS "${GLSLANG_INCLUDE_DIR}" "${SPIRV_INCLUDE_DIR}")
	set(GLSLANG_LIBRARIES glslang::glslang glslang::SPIRV)
endif()

target_compile_features(Acid.Graphics PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Graphics
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_graphics_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		PRIVATE
		# Since building locally from the submodules won't always create these vars.
		# We have to do a simple check if they exist or they will cause errors
		$<$<BOOL:${GLSLANG_INCLUDE_DIRS}>:${GLSLANG_INCLUDE_DIRS}>
		)
target_link_libraries(Acid.Graphics
		PUBLIC
		Acid.Core
		Acid.Context
		PRIVATE
		# More IMPORTED
		${GLSLANG_LIBRARIES}
		)

set_target_properties(Acid.Graphics PROPERTIES
		#INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "Acid"
		)

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Graphics/Export.hpp"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Graphics"
		)
foreach(_header IN LISTS _acid_graphics_headers _acid_graphics_third_party_headers)
    get_filename_component(_header_dir ${_header} DIRECTORY)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_header}
			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Graphics/${_header_dir}"
			)
endforeach()
# Install without an export since we're using 1 (or more) non-system libs
install(TARGETS Acid.Graphics
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
