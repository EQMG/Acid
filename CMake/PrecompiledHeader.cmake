# MIT License
# 
# Copyright (c) 2017 Sylvain Doremus
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

option(PROJECTS_USE_PRECOMPILED_HEADERS "Use precompiled headers" TRUE)

#--------------------------------------------------------------------------------------------------
# - Try to find precompiled headers support for GCC 3.4 and 4.x
# Once done this will define:
#
# Variable:
#   PCHSupport_FOUND
#
# Macro:
#   add_target_precompiled_header  targetName pchHeader pchSource targetCxxFlags cppFiles
#--------------------------------------------------------------------------------------------------
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

if(CMAKE_COMPILER_IS_GNUCXX)
	# Verifies if GCC supports precompiled header
	# Its version should be >= 3.4.0
	exec_program(${CMAKE_CXX_COMPILER} ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion OUTPUT_VARIABLE gcc_compiler_version)

	if(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
		set(PCHSupport_FOUND TRUE)
	elseif(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
		set(PCHSupport_FOUND TRUE)
	endif()

	set(PCH_INCLUDE_PREFIX -I)
else()
	if(MSVC)
		set(PCHSupport_FOUND TRUE) # for experimental msvc support
		set(PCH_INCLUDE_PREFIX /I)
	else()
		set(PCHSupport_FOUND FALSE)
	endif()     
endif()
#--------------------------------------------------------------------------------------------------
#
# Ensures the compile flags for the pch are the same that the regular compile flags
#
#--------------------------------------------------------------------------------------------------
macro(pch_get_compile_flags IN_COMPILE_FLAGS OUT_COMPILE_FLAGS)
	set(${OUT_COMPILE_FLAGS} ${IN_COMPILE_FLAGS})

	string(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" FLAGS_VAR_NAME)
	if(${FLAGS_VAR_NAME})
		list(APPEND ${OUT_COMPILE_FLAGS} ${${FLAGS_VAR_NAME}})
	endif(${FLAGS_VAR_NAME})

	# Add all include directives...
	get_directory_property(DIRINC INCLUDE_DIRECTORIES)
	foreach(ITEM ${DIRINC})
        list(APPEND ${OUT_COMPILE_FLAGS} -I${ITEM})
	endforeach(ITEM)

	# Add all definitions...
	get_target_property(COMPILER_FLAGS ${PCH_CURRENT_TARGET} COMPILE_FLAGS)
	if(COMPILER_FLAGS)
		list(APPEND ${OUT_COMPILE_FLAGS} ${COMPILER_FLAGS})
	endif()

	get_directory_property(DIRECTORY_FLAGS COMPILE_DEFINITIONS)
	if(DIRECTORY_FLAGS)
		foreach(FLAG ${DIRECTORY_FLAGS})
			list(APPEND ${OUT_COMPILE_FLAGS} -D${FLAG})
		endforeach(FLAG)
	endif(DIRECTORY_FLAGS)

	string(TOUPPER "COMPILE_DEFINITIONS_${CMAKE_BUILD_TYPE}" DEFS_PROP_NAME)
	get_directory_property(DIRECTORY_FLAGS ${DEFS_PROP_NAME})
	if(DIRECTORY_FLAGS)
		foreach(FLAG ${DIRECTORY_FLAGS})
			list(APPEND ${OUT_COMPILE_FLAGS} -D${FLAG})
		endforeach(FLAG)
	endif(DIRECTORY_FLAGS)

	get_target_property(TARGET_FLAGS ${PCH_CURRENT_TARGET} COMPILE_DEFINITIONS)
	if(TARGET_FLAGS)
		foreach(FLAG ${TARGET_FLAGS})
			list(APPEND ${OUT_COMPILE_FLAGS} -D${FLAG})
		endforeach(FLAG)
	endif(TARGET_FLAGS)

	get_target_property(TARGET_FLAGS ${PCH_CURRENT_TARGET} ${DEFS_PROP_NAME})
	if(TARGET_FLAGS)
		foreach(FLAG ${TARGET_FLAGS})
			list(APPEND ${OUT_COMPILE_FLAGS} -D${FLAG})
		endforeach(FLAG)
	endif(TARGET_FLAGS)

	separate_arguments(${OUT_COMPILE_FLAGS})
endmacro(pch_get_compile_flags)
#--------------------------------------------------------------------------------------------------
#
# Computes GCH compilation command, for GCC
#
#--------------------------------------------------------------------------------------------------
macro(pch_get_compile_command OUT_COMMAND input output)
	file(TO_NATIVE_PATH ${input} NATIVE_INPUT)
	file(TO_NATIVE_PATH ${output} NATIVE_OUTPUT)

	if(CMAKE_CXX_COMPILER_ARG1)
		# remove leading space in compiler argument
		string(REGEX REPLACE "^ +" "" PCHSUPPORT_COMPILER_CXX_ARG1 ${CMAKE_CXX_COMPILER_ARG1})
	else()
		set(PCHSUPPORT_COMPILER_CXX_ARG1 "")
	endif()
	set(${OUT_COMMAND} ${CMAKE_CXX_COMPILER} ${PCHSUPPORT_COMPILER_CXX_ARG1} ${PCH_COMPILE_FLAGS} -x c++-header -o ${NATIVE_OUTPUT} ${NATIVE_INPUT})
endmacro(pch_get_compile_command )
#--------------------------------------------------------------------------------------------------
#
# Adds the precompiled headers to the given target
#
#--------------------------------------------------------------------------------------------------
MACRO(add_target_precompiled_header TARGET_NAME PCH_HEADER PCH_SOURCE TARGET_CXX_FLAGS)
	if(PROJECTS_USE_PRECOMPILED_HEADERS)
		set(PCH_CURRENT_TARGET ${TARGET_NAME})
		get_filename_component(PCH_HEADER_NAME ${PCH_HEADER} NAME)
		get_filename_component(PCH_HEADER_PATH ${PCH_HEADER} PATH)
		get_filename_component(PCH_SOURCE_PATH ${PCH_SOURCE} PATH)
		set(PCH_INCLUDE_NAME ${PCH_HEADER_NAME})
		if(NOT "${PCH_HEADER_PATH}" STREQUAL "${PCH_SOURCE_PATH}")
			set(PCH_INCLUDE_NAME ${TARGET_NAME}/${PCH_INCLUDE_NAME})
		endif()
		if(MSVC)
			foreach(SRC ${ARGN})
				set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "${TARGET_CXX_FLAGS} /Yu${PCH_INCLUDE_NAME} /FI${PCH_INCLUDE_NAME}")
			endforeach()
			set_source_files_properties(${PCH_SOURCE} PROPERTIES COMPILE_FLAGS "${TARGET_CXX_FLAGS} /Yc${PCH_INCLUDE_NAME}")
		else()
			set(PCH_OUTPUT "${PCH_HEADER}.gch")
			pch_get_compile_flags("${TARGET_CXX_FLAGS}" PCH_COMPILE_FLAGS)
			pch_get_compile_command(PCH_COMMAND  ${PCH_HEADER} ${PCH_OUTPUT})
			add_custom_command(OUTPUT ${PCH_OUTPUT} COMMAND ${PCH_COMMAND} IMPLICIT_DEPENDS CXX ${PCH_HEADER})

			foreach(SRC ${ARGN})
				set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "${TARGET_CXX_FLAGS} -include ${PCH_HEADER} -Wl,--enable-large-address-aware")
				set_source_files_properties(${SRC} PROPERTIES OBJECT_DEPENDS ${PCH_OUTPUT})
			endforeach()
		endif()
	endif()
ENDMACRO(add_target_precompiled_header)
