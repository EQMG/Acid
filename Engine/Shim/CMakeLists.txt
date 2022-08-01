set(_component_name Shim)
set(_component_library Acid_${_component_name})
set(_component_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Gizmos/Gizmo.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Gizmos/Gizmos.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Gizmos/GizmosSubrender.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Gizmos/GizmoType.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Lights/Fog.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Lights/Light.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Materials/DefaultMaterial.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Materials/Material.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Materials/MaterialPipeline.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Meshes/Mesh.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Meshes/MeshesSubrender.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Emitters/CircleEmitter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Emitters/Emitter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Emitters/LineEmitter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Emitters/PointEmitter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Emitters/SphereEmitter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Particle.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Particles.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/ParticlesSubrender.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/ParticleSystem.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/ParticleType.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Deferred/DeferredSubrender.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/DofFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/LensflareFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/SsaoFilter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Shadows/ShadowBox.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Shadows/ShadowRender.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Shadows/Shadows.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Shadows/ShadowsSubrender.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Skyboxes/SkyboxMaterial.hpp
		)
set(_component_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Gizmos/Gizmo.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Gizmos/Gizmos.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Gizmos/GizmosSubrender.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Gizmos/GizmoType.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Lights/Fog.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Lights/Light.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Materials/DefaultMaterial.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Materials/MaterialPipeline.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Meshes/Mesh.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Meshes/MeshesSubrender.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Emitters/CircleEmitter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Emitters/LineEmitter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Emitters/PointEmitter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Emitters/SphereEmitter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Particle.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/Particles.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/ParticlesSubrender.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/ParticleSystem.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Particles/ParticleType.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Deferred/DeferredSubrender.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/DofFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/LensflareFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Post/Filters/SsaoFilter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Shadows/ShadowBox.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Shadows/ShadowRender.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Shadows/Shadows.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Shadows/ShadowsSubrender.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Skyboxes/SkyboxMaterial.cpp
		)
set(_component_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp
		)

add_library(${_component_library}
        ${_component_headers}
        ${_component_sources}
		${_component_generated_headers}
        )
add_library(Acid::${_component_name} ALIAS ${_component_library})

include(GenerateExportHeader)
generate_export_header(${_component_library} EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp")

target_compile_features(${_component_library} PUBLIC cxx_std_20 c_std_11)
target_include_directories(${_component_library}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}>
		)
target_link_libraries(${_component_library}
		PUBLIC
		Acid::Scene
		Acid::Graphics
		)
set_target_properties(${_component_library}
		PROPERTIES
		FOLDER "Acid"
		EXPORT_NAME "${_component_name}"
		)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_component_generated_headers})

install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_SOURCE_DIR}" "${_component_headers}")
install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_component_generated_headers}")

set(ACID_LIBRARIES "${ACID_LIBRARIES};${_component_library}" CACHE INTERNAL "")
