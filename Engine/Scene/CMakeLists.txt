set(_acid_scene_headers
		Maths/Frustum.hpp
		Maths/Ray.hpp
		Maths/Transform.hpp
		Scenes/Camera.hpp
		Scenes/Component.hpp
		Scenes/Entity.hpp
		Scenes/EntityHolder.hpp
		Scenes/EntityPrefab.hpp
		Scenes/Scene.hpp
		Scenes/Scenes.hpp
		Scenes/System.hpp
		Scenes/SystemHolder.hpp
		)
set(_acid_scene_sources
		Maths/Frustum.cpp
		Maths/Ray.cpp
		Maths/Transform.cpp
		Scenes/Entity.cpp
		Scenes/EntityHolder.cpp
		Scenes/EntityPrefab.cpp
		Scenes/Scene.cpp
		Scenes/Scenes.cpp
		Scenes/SystemHolder.cpp
		)

set(_acid_scene_third_party_headers
		)
set(_acid_scene_third_party_sources
		)
set(_acid_scene_third_party_includes
		)

add_library(Acid.Scene
        ${_acid_scene_headers}
        ${_acid_scene_sources}
        ${_acid_scene_third_party_headers}
        ${_acid_scene_third_party_sources}
        )

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Scene EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Scene/Export.hpp")

target_compile_features(Acid.Scene PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Scene
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_scene_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		)
target_link_libraries(Acid.Scene
		PUBLIC
		Acid.Core
		)
