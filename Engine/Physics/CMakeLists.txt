set(_acid_physics_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/CapsuleCollider.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/Collider.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/ConeCollider.hpp
		#${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/ConvexHullCollider.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/CubeCollider.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/CylinderCollider.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/HeightfieldCollider.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/SphereCollider.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/CollisionObject.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Force.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/KinematicCharacter.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Physics.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Rigidbody.hpp
		)
set(_acid_physics_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/CapsuleCollider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/Collider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/ConeCollider.cpp
		#${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/ConvexHullCollider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/CubeCollider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/CylinderCollider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/HeightfieldCollider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/SphereCollider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/CollisionObject.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Force.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/KinematicCharacter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Physics.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Rigidbody.cpp
		)
set(_acid_physics_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/PhysicsExport.hpp
		)

add_library(Acid.Physics
        ${_acid_physics_headers}
        ${_acid_physics_sources}
		${_acid_physics_generated_headers}
        )

include(GenerateExportHeader)
generate_export_header(Acid.Physics EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/PhysicsExport.hpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_physics_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_physics_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_acid_physics_generated_headers})

if(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
	set(CMAKE_DISABLE_FIND_PACKAGE_Bullet TRUE CACHE INTERNAL "")
endif()
#find_package(Bullet 3.17 QUIET)
if(NOT BULLET_FOUND)
	if(MSVC)
		set(USE_MSVC_INCREMENTAL_LINKING ON CACHE INTERNAL "")
		set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE INTERNAL "")
	endif()
	set(USE_GRAPHICAL_BENCHMARK OFF CACHE INTERNAL "Use Graphical Benchmark")
	set(BUILD_CPU_DEMOS OFF CACHE INTERNAL "Build original Bullet CPU examples")
	set(USE_GLUT OFF CACHE INTERNAL "Use Glut")
	set(BUILD_BULLET3 OFF CACHE INTERNAL "Set when you want to build Bullet 3")
	set(BUILD_PYBULLET OFF CACHE INTERNAL "Set when you want to build pybullet (Python bindings for Bullet)")
	set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "Set when you want to build the Bullet 2 demos")
	set(BUILD_EXTRAS OFF CACHE INTERNAL "Set when you want to build the extras")
	set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "Build Unit Tests")
	set(INSTALL_CMAKE_FILES OFF CACHE INTERNAL "Install generated CMake files")
	set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
	set(BUILD_SHARED_LIBS OFF)
	add_subdirectory(${ACID_THIRD_PARTY_DIR}/bullet3 bullet3)
	set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")

	set_target_properties(Bullet3Common PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletCollision PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletDynamics PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletInverseDynamics PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletSoftBody PROPERTIES FOLDER "External/bullet")
	set_target_properties(LinearMath PROPERTIES FOLDER "External/bullet")

	set(BULLET_INCLUDE_DIRS "${ACID_THIRD_PARTY_DIR}/bullet3/src")
	set(BULLET_LIBRARIES BulletSoftBody BulletDynamics BulletCollision LinearMath)
endif()

set_target_properties(Acid.Physics PROPERTIES FOLDER "Acid")
target_compile_features(Acid.Physics PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Physics
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Physics>
		PRIVATE
		$<$<BOOL:${BULLET_INCLUDE_DIRS}>:${BULLET_INCLUDE_DIRS}>
		)
target_link_libraries(Acid.Physics
		PUBLIC
		Acid.Core
		Acid.Scene
		PRIVATE
		${BULLET_LIBRARIES}
		)

install(TARGETS Acid.Physics
		EXPORT AcidPhysicsExport
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
install(EXPORT AcidPhysicsExport
		FILE AcidPhysicsConfig.cmake
		#NAMESPACE acid::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
install_headers_with_dir("Physics" "${CMAKE_CURRENT_SOURCE_DIR}" "${_acid_physics_headers}")
install_headers_with_dir("Physics" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_acid_physics_generated_headers}")
