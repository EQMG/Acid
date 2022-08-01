set(_component_name Scene)
set(_component_library Acid_${_component_name})
set(_component_headers
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
set(_component_sources
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
		Acid::Core
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
