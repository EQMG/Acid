set(_component_name Physics)
set(_component_library Acid_${_component_name})
set(_component_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/CapsuleCollider.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/Collider.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/ConeCollider.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/ConvexHullCollider.hpp
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
set(_component_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/CapsuleCollider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/Collider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/ConeCollider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Physics/Colliders/ConvexHullCollider.cpp
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

if(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
	set(CMAKE_DISABLE_FIND_PACKAGE_Bullet TRUE)
endif()
find_package(Bullet 3.17 QUIET)
if(NOT BULLET_FOUND)
	if(MSVC)
		set(USE_MSVC_INCREMENTAL_LINKING ON CACHE BOOL "Use MSVC Incremental Linking")
		set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE BOOL "Use MSVC Runtime Library DLL")
	endif()
	set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "Use Graphical Benchmark")
	set(BUILD_CPU_DEMOS OFF CACHE BOOL "Build original Bullet CPU examples")
	set(USE_GLUT OFF CACHE BOOL "Use Glut")
	set(BUILD_BULLET3 OFF CACHE BOOL "Set when you want to build Bullet 3")
	set(BUILD_PYBULLET OFF CACHE BOOL "Set when you want to build pybullet (Python bindings for Bullet)")
	set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Set when you want to build the Bullet 2 demos")
	set(BUILD_EXTRAS OFF CACHE BOOL "Set when you want to build the extras")
	set(BUILD_UNIT_TESTS OFF CACHE BOOL "Build Unit Tests")
	set(INSTALL_CMAKE_FILES OFF CACHE BOOL "Install generated CMake files")

	set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
	set(BUILD_SHARED_LIBS OFF)
	add_subdirectory(${THIRD_PARTY_SOURCE_DIR}/bullet3 ${THIRD_PARTY_BINARY_DIR}/bullet3)
	set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")

	set_target_properties(Bullet3Common PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletCollision PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletDynamics PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletInverseDynamics PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletSoftBody PROPERTIES FOLDER "External/bullet")
	set_target_properties(LinearMath PROPERTIES FOLDER "External/bullet")

	set(BULLET_INCLUDE_DIRS "${THIRD_PARTY_SOURCE_DIR}/bullet3/src")
	set(BULLET_LIBRARIES BulletSoftBody BulletDynamics BulletCollision LinearMath)
endif()

target_compile_features(${_component_library} PUBLIC c_std_11 cxx_std_17)
target_include_directories(${_component_library}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}>
		PRIVATE
		$<BUILD_INTERFACE:$<$<BOOL:${BULLET_INCLUDE_DIRS}>:${BULLET_INCLUDE_DIRS}>>
		)
target_link_libraries(${_component_library}
		PUBLIC
		Acid::Scene
		PRIVATE
		${BULLET_LIBRARIES}
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
