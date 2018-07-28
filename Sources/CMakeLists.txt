include(CMakeSources.cmake)
#project(Acid)

set(ACID_INCLUDES "${PROJECT_SOURCE_DIR}/Sources/")

add_library(Acid ${LIB_TYPE} ${ACID_SOURCES})
add_dependencies(Acid SPIRV BulletDynamics)

set_target_properties(Acid PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        FOLDER "Acid")

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(Acid PROPERTIES PREFIX "")
    set_target_properties(Acid PROPERTIES DEBUG_POSTFIX "")
endif()

if(BUILD_SHARED_LIBS)
    target_compile_definitions(Acid PUBLIC
            -DACID_EXPORTS
            )
else()
    target_compile_definitions(Acid PUBLIC
            -DACID_STATICLIB
            )
endif()

find_package(Threads REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenAL REQUIRED)

if(UNIX AND NOT APPLE)
    find_package(XCB REQUIRED)
    target_include_directories(Acid PUBLIC ${XCB_INCLUDE_DIRS})
    target_link_libraries(Acid PUBLIC ${XCB_LIBRARIES})
elseif(UNIX AND APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    target_compile_options(Acid PRIVATE -std=c99 -x objective-c -fno-objc-arc -Wall -Wno-unused-function -Wno-unused-const-variable)
    target_link_libraries(Acid PUBLIC ${COCOA_LIBRARY})
endif()

target_include_directories(Acid PUBLIC ${VULKAN_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR} ${GLSLANG_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS} ${ACID_INCLUDES})
target_link_libraries(Acid PUBLIC ${VULKAN_LIBRARY} ${OPENAL_LIBRARY} ${GLSLANG_LIBRARIES} ${BULLET_LIBRARIES})

# Install
if(ACID_INSTALL)
    install(TARGETS Acid
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )

    foreach(file ${ACID_HEADERS_})
        get_filename_component(dir ${file} DIRECTORY)
        install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/acid/${dir})
    endforeach()
endif()
