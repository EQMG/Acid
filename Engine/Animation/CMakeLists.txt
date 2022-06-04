set(_acid_animation_headers
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
set(_acid_animation_sources
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
set(_acid_animation_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/AnimationExport.hpp
		)

add_library(Acid.Animation
        ${_acid_animation_headers}
        ${_acid_animation_sources}
		${_acid_animation_generated_headers}
        )

include(GenerateExportHeader)
generate_export_header(Acid.Animation EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/AnimationExport.hpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_animation_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_animation_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_acid_animation_generated_headers})

set_target_properties(Acid.Animation PROPERTIES FOLDER "Acid")
target_compile_features(Acid.Animation PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Animation
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Animation>
		)
target_link_libraries(Acid.Animation
		PUBLIC
		Acid.Core
		Acid.Shim
		)

install(TARGETS Acid.Animation
		EXPORT AcidAnimationExport
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
install(EXPORT AcidAnimationExport
		FILE AcidAnimationConfig.cmake
		#NAMESPACE acid::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
install_headers_with_dir("Animation" "${CMAKE_CURRENT_SOURCE_DIR}" "${_acid_animation_headers}")
install_headers_with_dir("Animation" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_acid_animation_generated_headers}")
