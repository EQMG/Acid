set(_acid_shim_headers
		Gizmos/Gizmo.hpp
		Gizmos/Gizmos.hpp
		Gizmos/GizmosSubrender.hpp
		Gizmos/GizmoType.hpp
		Lights/Fog.hpp
		Lights/Light.hpp
		Materials/DefaultMaterial.hpp
		Materials/Material.hpp
		Materials/MaterialPipeline.hpp
		Meshes/Mesh.hpp
		Meshes/MeshesSubrender.hpp
		Particles/Emitters/CircleEmitter.hpp
		Particles/Emitters/Emitter.hpp
		Particles/Emitters/LineEmitter.hpp
		Particles/Emitters/PointEmitter.hpp
		Particles/Emitters/SphereEmitter.hpp
		Particles/Particle.hpp
		Particles/Particles.hpp
		Particles/ParticlesSubrender.hpp
		Particles/ParticleSystem.hpp
		Particles/ParticleType.hpp
		Post/Deferred/DeferredSubrender.hpp
		Post/Filters/DofFilter.hpp
		Post/Filters/LensflareFilter.hpp
		Post/Filters/SsaoFilter.hpp
		Shadows/ShadowBox.hpp
		Shadows/ShadowRender.hpp
		Shadows/Shadows.hpp
		Shadows/ShadowsSubrender.hpp
		Skyboxes/SkyboxMaterial.hpp
		)
set(_acid_shim_sources
		Gizmos/Gizmo.cpp
		Gizmos/Gizmos.cpp
		Gizmos/GizmosSubrender.cpp
		Gizmos/GizmoType.cpp
		Lights/Fog.cpp
		Lights/Light.cpp
		Materials/DefaultMaterial.cpp
		Materials/MaterialPipeline.cpp
		Meshes/Mesh.cpp
		Meshes/MeshesSubrender.cpp
		Particles/Emitters/CircleEmitter.cpp
		Particles/Emitters/LineEmitter.cpp
		Particles/Emitters/PointEmitter.cpp
		Particles/Emitters/SphereEmitter.cpp
		Particles/Particle.cpp
		Particles/Particles.cpp
		Particles/ParticlesSubrender.cpp
		Particles/ParticleSystem.cpp
		Particles/ParticleType.cpp
		Post/Deferred/DeferredSubrender.cpp
		Post/Filters/DofFilter.cpp
		Post/Filters/LensflareFilter.cpp
		Post/Filters/SsaoFilter.cpp
		Shadows/ShadowBox.cpp
		Shadows/ShadowRender.cpp
		Shadows/Shadows.cpp
		Shadows/ShadowsSubrender.cpp
		Skyboxes/SkyboxMaterial.cpp
		)

set(_acid_shim_third_party_headers
		)
set(_acid_shim_third_party_sources
		)
set(_acid_shim_third_party_includes
		)

add_library(Acid.Shim
        ${_acid_shim_headers}
        ${_acid_shim_sources}
        ${_acid_shim_third_party_headers}
        ${_acid_shim_third_party_sources}
        )

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Shim EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Shim/Export.hpp")

target_compile_features(Acid.Shim PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Shim
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_shim_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		)
target_link_libraries(Acid.Shim
		PUBLIC
		Acid.Core
		Acid.Scene
		Acid.Graphics
		)

set_target_properties(Acid.Shim PROPERTIES
		#INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "Acid"
		)

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Shim/Export.hpp"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Shim"
		)
foreach(_header IN LISTS _acid_shim_headers _acid_shim_third_party_headers)
    get_filename_component(_header_dir ${_header} DIRECTORY)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_header}
			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Shim/${_header_dir}"
			)
endforeach()
# Install without an export since we're using 1 (or more) non-system libs
install(TARGETS Acid.Shim
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
