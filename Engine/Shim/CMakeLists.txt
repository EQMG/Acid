set(_acid_shim_headers
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
set(_acid_shim_sources
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
set(_acid_shim_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/ShimExport.hpp
		)

add_library(Acid.Shim
        ${_acid_shim_headers}
        ${_acid_shim_sources}
		${_acid_shim_generated_headers}
        )

include(GenerateExportHeader)
generate_export_header(Acid.Shim EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/ShimExport.hpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_shim_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_shim_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_acid_shim_generated_headers})

set_target_properties(Acid.Shim PROPERTIES FOLDER "Acid")
target_compile_features(Acid.Shim PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Shim
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Shim>
		)
target_link_libraries(Acid.Shim
		PUBLIC
		Acid.Core
		Acid.Scene
		Acid.Graphics
		)

install(TARGETS Acid.Shim
		EXPORT AcidShimExport
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
install(EXPORT AcidShimExport
		FILE AcidShimConfig.cmake
		#NAMESPACE acid::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
install_headers_with_dir("Shim" "${CMAKE_CURRENT_SOURCE_DIR}" "${_acid_shim_headers}")
install_headers_with_dir("Shim" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_acid_shim_generated_headers}")
