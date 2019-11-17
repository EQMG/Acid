include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

find_library(GLSLANG_LIBRARY_RELEASE NAMES glslang PATH_SUFFIXES lib)
find_library(OGLCOMPILER_LIBRARY_RELEASE NAMES OGLCompiler PATH_SUFFIXES lib)
find_library(OSDEPENDENT_LIBRARY_RELEASE NAMES OSDependent PATH_SUFFIXES lib)
find_library(HLSL_LIBRARY_RELEASE NAMES HLSL PATH_SUFFIXES lib)
find_library(SPIRV_LIBRARY_RELEASE NAMES SPIRV PATH_SUFFIXES lib)
find_library(GLSLANG_LIBRARY_DEBUG NAMES glslangd PATH_SUFFIXES debug/lib)
find_library(OGLCOMPILER_LIBRARY_DEBUG NAMES OGLCompilerd PATH_SUFFIXES debug/lib)
find_library(OSDEPENDENT_LIBRARY_DEBUG NAMES OSDependentd PATH_SUFFIXES debug/lib)
find_library(SPIRV_LIBRARY_DEBUG NAMES SPIRVd PATH_SUFFIXES debug/lib)
find_library(HLSL_LIBRARY_DEBUG NAMES HLSLd PATH_SUFFIXES debug/lib)
find_path(GLSLANG_INCLUDE_DIR NAMES glslang/Public/ShaderLang.h PATH_SUFFIXES include)
find_path(SPIRV_INCLUDE_DIR NAMES SPIRV/spirv.hpp PATH_SUFFIXES include)
find_program(GLSLANG_VALIDATOR_EXE NAMES glslangValidator PATH_SUFFIXES bin)

select_library_configurations(GLSLANG)
select_library_configurations(OGLCOMPILER)
select_library_configurations(OSDEPENDENT)
select_library_configurations(SPIRV)
select_library_configurations(HLSL)

find_package_handle_standard_args(glslang
		DEFAULT_MSG
		GLSLANG_LIBRARY
		OGLCOMPILER_LIBRARY
		OSDEPENDENT_LIBRARY
		SPIRV_LIBRARY
		HLSL_LIBRARY
		GLSLANG_INCLUDE_DIR
		SPIRV_INCLUDE_DIR
		)

add_library(glslang::OGLCompiler STATIC IMPORTED)
if(OGLCOMPILER_LIBRARY_RELEASE)
	set_property(TARGET glslang::OGLCompiler APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
	set_target_properties(glslang::OGLCompiler PROPERTIES
			IMPORTED_LOCATION_RELEASE ${OGLCOMPILER_LIBRARY_RELEASE}
			)
endif()
if(OGLCOMPILER_LIBRARY_DEBUG)
	set_property(TARGET glslang::OGLCompiler APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
	set_target_properties(glslang::OGLCompiler PROPERTIES
			IMPORTED_LOCATION_DEBUG ${OGLCOMPILER_LIBRARY_DEBUG}
			)
endif()

add_library(glslang::OSDependent STATIC IMPORTED)
if(OSDEPENDENT_LIBRARY_RELEASE)
	set_property(TARGET glslang::OSDependent APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
	set_target_properties(glslang::OSDependent PROPERTIES
			IMPORTED_LOCATION_RELEASE ${OSDEPENDENT_LIBRARY_RELEASE}
			)
endif()
if(OSDEPENDENT_LIBRARY_DEBUG)
	set_property(TARGET glslang::OSDependent APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
	set_target_properties(glslang::OSDependent PROPERTIES
			IMPORTED_LOCATION_DEBUG ${OSDEPENDENT_LIBRARY_DEBUG}
			)
endif()

add_library(glslang::HLSL STATIC IMPORTED)
if(HLSL_LIBRARY_RELEASE)
	set_property(TARGET glslang::HLSL APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
	set_target_properties(glslang::HLSL PROPERTIES
		IMPORTED_LOCATION_RELEASE ${HLSL_LIBRARY_RELEASE})
endif()
if(HLSL_LIBRARY_DEBUG)
	set_property(TARGET glslang::HLSL APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
	set_target_properties(glslang::HLSL PROPERTIES
			IMPORTED_LOCATION_DEBUG ${HLSL_LIBRARY_DEBUG}
			)
endif()

add_library(glslang::glslang STATIC IMPORTED)
if(GLSLANG_LIBRARY_RELEASE)
	set_property(TARGET glslang::glslang APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
	set_target_properties(glslang::glslang PROPERTIES
			IMPORTED_LOCATION_RELEASE ${GLSLANG_LIBRARY_RELEASE}
			)
endif()
if(GLSLANG_LIBRARY_DEBUG)
	set_property(TARGET glslang::glslang APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
	set_target_properties(glslang::glslang PROPERTIES
			IMPORTED_LOCATION_DEBUG ${GLSLANG_LIBRARY_DEBUG}
			)
endif()
set_target_properties(glslang::glslang PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${GLSLANG_INCLUDE_DIR})
target_link_libraries(glslang::glslang INTERFACE glslang::OGLCompiler glslang::OSDependent glslang::HLSL)

add_library(glslang::SPIRV STATIC IMPORTED)
if(SPIRV_LIBRARY_RELEASE)
	set_property(TARGET glslang::SPIRV APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
	set_target_properties(glslang::SPIRV PROPERTIES
			IMPORTED_LOCATION_RELEASE ${SPIRV_LIBRARY_RELEASE}
			)
endif()
if(SPIRV_LIBRARY_DEBUG)
	set_property(TARGET glslang::SPIRV APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
	set_target_properties(glslang::SPIRV PROPERTIES
			IMPORTED_LOCATION_DEBUG ${SPIRV_LIBRARY_DEBUG}
			)
endif()
set_target_properties(glslang::SPIRV PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SPIRV_INCLUDE_DIR})
target_link_libraries(glslang::SPIRV INTERFACE glslang::glslang)

find_package_handle_standard_args(glslangValidator
		DEFAULT_MSG
		GLSLANG_VALIDATOR_EXE
		)

add_executable(glslangValidator IMPORTED)
set_target_properties(glslangValidator PROPERTIES IMPORTED_LOCATION ${GLSLANG_VALIDATOR_EXE})

mark_as_advanced(
		GLSLANG_LIBRARY
		OGLCOMPILER_LIBRARY
		OSDEPENDENT_LIBRARY
		HLSL_LIBRARY
		SPIRV_LIBRARY
		)
