set(_component_name Animation)
set(_component_library Acid.${_component_name})
set(_component_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/AnimatedMesh.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Animation/Animation.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Animation/AnimationLoader.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Animation/JointTransform.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Animation/Keyframe.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Animator.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Geometry/GeometryLoader.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Geometry/VertexAnimated.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Skeleton/Joint.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Skeleton/SkeletonLoader.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Skin/SkinLoader.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Skin/VertexWeights.hpp
		)
set(_component_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/AnimatedMesh.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Animation/Animation.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Animation/AnimationLoader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Animation/JointTransform.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Animation/Keyframe.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Animator.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Geometry/GeometryLoader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Skeleton/Joint.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Skeleton/SkeletonLoader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Skin/SkinLoader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/Skin/VertexWeights.cpp
		)
set(_component_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp
		)

add_library(${_component_library}
        ${_component_headers}
        ${_component_sources}
		${_component_generated_headers}
        )

include(GenerateExportHeader)
generate_export_header(${_component_library} EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp")

set_target_properties(${_component_library} PROPERTIES FOLDER "Acid")
target_compile_features(${_component_library} PUBLIC c_std_11 cxx_std_17)
target_include_directories(${_component_library}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}>
		)
target_link_libraries(${_component_library}
		PUBLIC
		Acid.Core
		Acid.Scene
		Acid.Shim
		)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_component_generated_headers})

install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_SOURCE_DIR}" "${_component_headers}")
install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_component_generated_headers}")

set(ACID_INSTALL_TARGETS ${ACID_INSTALL_TARGETS} ";${_component_library}" PARENT_SCOPE)
