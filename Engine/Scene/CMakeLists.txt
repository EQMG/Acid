set(_acid_scene_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Frustum.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Ray.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Transform.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/Camera.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/Component.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/Entity.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/EntityHolder.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/EntityPrefab.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/Scene.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/Scenes.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/System.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/SystemHolder.hpp
		)
set(_acid_scene_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Frustum.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Ray.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Transform.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/Entity.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/EntityHolder.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/EntityPrefab.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/Scene.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/Scenes.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/SystemHolder.cpp
		)
set(_acid_scene_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/SceneExport.hpp
		)

add_library(Acid.Scene
        ${_acid_scene_headers}
        ${_acid_scene_sources}
		${_acid_scene_generated_headers}
        )

include(GenerateExportHeader)
generate_export_header(Acid.Scene EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/SceneExport.hpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_scene_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_scene_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_acid_scene_generated_headers})

set_target_properties(Acid.Scene PROPERTIES FOLDER "Acid")
target_compile_features(Acid.Scene PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Scene
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Scene>
		)
target_link_libraries(Acid.Scene
		PUBLIC
		Acid.Core
		)

install(TARGETS Acid.Scene
		EXPORT AcidSceneExport
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
install(EXPORT AcidSceneExport
		FILE AcidSceneConfig.cmake
		#NAMESPACE acid::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
install_headers_with_dir("Scene" "${CMAKE_CURRENT_SOURCE_DIR}" "${_acid_scene_headers}")
install_headers_with_dir("Scene" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_acid_scene_generated_headers}")
