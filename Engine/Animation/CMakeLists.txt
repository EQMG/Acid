set(_acid_animation_headers
		Animations/AnimatedMesh.hpp
		Animations/Animation/Animation.hpp
		Animations/Animation/AnimationLoader.hpp
		Animations/Animation/JointTransform.hpp
		Animations/Animation/Keyframe.hpp
		Animations/Animator.hpp
		Animations/Geometry/GeometryLoader.hpp
		Animations/Geometry/VertexAnimated.hpp
		Animations/Skeleton/Joint.hpp
		Animations/Skeleton/SkeletonLoader.hpp
		Animations/Skin/SkinLoader.hpp
		Animations/Skin/VertexWeights.hpp
		)
set(_acid_animation_sources
		Animations/AnimatedMesh.cpp
		Animations/Animation/Animation.cpp
		Animations/Animation/AnimationLoader.cpp
		Animations/Animation/JointTransform.cpp
		Animations/Animation/Keyframe.cpp
		Animations/Animator.cpp
		Animations/Geometry/GeometryLoader.cpp
		Animations/Skeleton/Joint.cpp
		Animations/Skeleton/SkeletonLoader.cpp
		Animations/Skin/SkinLoader.cpp
		Animations/Skin/VertexWeights.cpp
		)

set(_acid_animation_third_party_headers
		)
set(_acid_animation_third_party_sources
		)
set(_acid_animation_third_party_includes
		)

add_library(Acid.Animation
        ${_acid_animation_headers}
        ${_acid_animation_sources}
        ${_acid_animation_third_party_headers}
        ${_acid_animation_third_party_sources}
        )

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Animation EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Animation/Export.hpp")

target_compile_features(Acid.Animation PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Animation
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_animation_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		)
target_link_libraries(Acid.Animation
		PUBLIC
		Acid.Core
		Acid.Shim
		)

set_target_properties(Acid.Animation PROPERTIES
		#INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "Acid"
		)

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Animation/Export.hpp"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Animation"
		)
foreach(_header IN LISTS _acid_animation_headers _acid_animation_third_party_headers)
    get_filename_component(_header_dir ${_header} DIRECTORY)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_header}
			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Animation/${_header_dir}"
			)
endforeach()
# Install without an export since we're using 1 (or more) non-system libs
install(TARGETS Acid.Animation
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
