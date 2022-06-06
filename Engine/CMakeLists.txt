# Installs headers, preserving their file structure
function(install_headers_with_dir _component_name _source_dir _header_list)
	foreach(_header IN ITEMS ${_header_list})
		file(RELATIVE_PATH _rel_header "${_source_dir}" ${_header})
	    get_filename_component(_header_dir ${_rel_header} DIRECTORY)
		install(FILES ${_header}
				DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}/${_header_dir}"
				)
	endforeach()
endfunction()

# Uses Git to find the current git branch and commit
find_package(Git REQUIRED)

#execute_process(
#		COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#		COMMAND_ERROR_IS_FATAL ANY
#		)
execute_process(
		COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE ACID_GIT_BRANCH
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
		)
execute_process(
		COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE ACID_GIT_COMMIT_HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
		)

# Acid components
add_subdirectory(Core)

option(ACID_NETWORK "Build Acid networking component" ON)
if(ACID_NETWORK)
    add_subdirectory(Network)
endif()

option(ACID_CONTEXT "Build Acid window and input component" ON)
if(ACID_CONTEXT)
    add_subdirectory(Context)
endif()

option(ACID_GRAPHICS "Build Acid graphics component" ON)
if(ACID_GRAPHICS)
    add_subdirectory(Graphics)
endif()

option(ACID_UI "Build Acid UI component" ON)
if(ACID_UI)
    add_subdirectory(Ui)
endif()

option(ACID_SCENE "Build Acid entity component system, scene component" ON)
if(ACID_SCENE)
    add_subdirectory(Scene)
endif()

option(ACID_SOUND "Build Acid audio component" ON)
if(ACID_SOUND)
    add_subdirectory(Sound)
endif()

option(ACID_PHYSICS "Build Acid scene physics system component" ON)
if(ACID_PHYSICS)
    add_subdirectory(Physics)
endif()

option(ACID_SHIM "Build Acid miscellaneous scene systems and subrenderers component" ON)
if(ACID_SHIM)
    add_subdirectory(Shim)
endif()

option(ACID_ANIMATION "Build Acid collada animation entity component" ON)
if(ACID_ANIMATION)
    add_subdirectory(Animation)
endif()

if(ACID_INSTALL_RESOURCES)
	# Install resources for end-user usage because many source files use these
	install(DIRECTORY "${ACID_RESOURCES_DIR}"
			# Example: this will install the Resources dir to /usr/share/Acid/Resources on Linux
			DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}"
			)
endif()
install(TARGETS ${ACID_LIBRARIES}
		# Auto-generates an export to install
		EXPORT AcidTargets
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
# Install the auto-generated export
install(EXPORT AcidTargets
		NAMESPACE Acid::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
# Defines write_basic_package_version_file
include(CMakePackageConfigHelpers)
# Dynamically writes the path to the resources dir (if installed) into the AcidConfig.cmake file
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/AcidConfig.cmake.in"
		"${PROJECT_BINARY_DIR}/AcidConfig.cmake"
		INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		PATH_VARS CMAKE_INSTALL_DATADIR
		NO_CHECK_REQUIRED_COMPONENTS_MACRO
		)
# Auto-generates a version file based on the current version specified in project()
write_basic_package_version_file("${PROJECT_BINARY_DIR}/AcidConfigVersion.cmake"
		COMPATIBILITY SameMajorVersion
		)
# Install the auto-generated version file and AcidConfig
install(FILES
		"${PROJECT_BINARY_DIR}/AcidConfig.cmake"
		"${PROJECT_BINARY_DIR}/AcidConfigVersion.cmake"
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
