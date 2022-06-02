set(_acid_physics_headers
		Physics/Colliders/CapsuleCollider.hpp
		Physics/Colliders/Collider.hpp
		Physics/Colliders/ConeCollider.hpp
		#Physics/Colliders/ConvexHullCollider.hpp
		Physics/Colliders/CubeCollider.hpp
		Physics/Colliders/CylinderCollider.hpp
		Physics/Colliders/HeightfieldCollider.hpp
		Physics/Colliders/SphereCollider.hpp
		Physics/CollisionObject.hpp
		Physics/Force.hpp
		Physics/KinematicCharacter.hpp
		Physics/Physics.hpp
		Physics/Rigidbody.hpp
		)
set(_acid_physics_sources
		Physics/Colliders/CapsuleCollider.cpp
		Physics/Colliders/Collider.cpp
		Physics/Colliders/ConeCollider.cpp
		#Physics/Colliders/ConvexHullCollider.cpp
		Physics/Colliders/CubeCollider.cpp
		Physics/Colliders/CylinderCollider.cpp
		Physics/Colliders/HeightfieldCollider.cpp
		Physics/Colliders/SphereCollider.cpp
		Physics/CollisionObject.cpp
		Physics/Force.cpp
		Physics/KinematicCharacter.cpp
		Physics/Physics.cpp
		Physics/Rigidbody.cpp
		)

set(_acid_physics_third_party_headers
		)
set(_acid_physics_third_party_sources
		)
set(_acid_physics_third_party_includes
		)

add_library(Acid.Physics
        ${_acid_physics_headers}
        ${_acid_physics_sources}
        ${_acid_physics_third_party_headers}
        ${_acid_physics_third_party_sources}
        )

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Physics EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Physics/Export.hpp")

if(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
	set(CMAKE_DISABLE_FIND_PACKAGE_Bullet TRUE CACHE INTERNAL "")
endif()
find_package(Bullet 3.17 QUIET)
if(NOT BULLET_FOUND)
	# On MSVC shared mode must be disabled with bullet currently
	set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
	if(MSVC)
		set(USE_MSVC_INCREMENTAL_LINKING ON CACHE INTERNAL "")
		set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE INTERNAL "")
		set(BUILD_SHARED_LIBS OFF)
	endif()
	set(USE_GRAPHICAL_BENCHMARK OFF CACHE INTERNAL "Use Graphical Benchmark")
	set(BUILD_CPU_DEMOS OFF CACHE INTERNAL "Build original Bullet CPU examples")
	set(USE_GLUT OFF CACHE INTERNAL "Use Glut")
	set(BUILD_BULLET3 OFF CACHE INTERNAL "Set when you want to build Bullet 3")
	set(BUILD_PYBULLET OFF CACHE INTERNAL "Set when you want to build pybullet (Python bindings for Bullet)")
	set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "Set when you want to build the Bullet 2 demos")
	set(BUILD_EXTRAS OFF CACHE INTERNAL "Set when you want to build the extras")
	set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "Build Unit Tests")
	add_subdirectory(../../External/bullet3 bullet3)
	# Reset back to value before MSVC fix
	set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")

	set_target_properties(Bullet3Common PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletCollision PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletDynamics PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletInverseDynamics PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletSoftBody PROPERTIES FOLDER "External/bullet")
	set_target_properties(LinearMath PROPERTIES FOLDER "External/bullet")

	set(BULLET_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../../External/bullet3/src")
	set(BULLET_LIBRARIES BulletSoftBody BulletDynamics BulletCollision LinearMath)
endif()

target_compile_features(Acid.Physics PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Physics
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_physics_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		PRIVATE
		# Since building locally from the submodules won't always create these vars.
		# We have to do a simple check if they exist or they will cause errors
		$<$<BOOL:${BULLET_INCLUDE_DIRS}>:${BULLET_INCLUDE_DIRS}>
		)
target_link_libraries(Acid.Physics
		PUBLIC
		Acid.Core
		Acid.Scene
		PRIVATE
		# More IMPORTED
		${BULLET_LIBRARIES}
		)

set_target_properties(Acid.Physics PROPERTIES
		#INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "Acid"
		)

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Physics/Export.hpp"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Physics"
		)
foreach(_header IN LISTS _acid_physics_headers _acid_physics_third_party_headers)
    get_filename_component(_header_dir ${_header} DIRECTORY)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_header}
			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Physics/${_header_dir}"
			)
endforeach()
# Install without an export since we're using 1 (or more) non-system libs
install(TARGETS Acid.Physics
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
