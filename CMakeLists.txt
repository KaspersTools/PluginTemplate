cmake_minimum_required(VERSION 3.24.4)
project(HUMMINGBIRD_PLUGIN_TEMPLATE)
set(CMAKE_CXX_STANDARD 23)

include(cmake/ProjectInfo.cmake)

message(STATUS "HUMMINGBIRD_PLUGIN_${PROJECT_NAME}_DIR: ${PROJECT_DIR}")
add_library(${PLUGIN_NAME} SHARED
        PluginTemplate/src/PluginTemplate.cpp
        PluginTemplate/src/PluginTemplate.h
        PluginTemplate/src/PluginTemplateWindow.h
)

if (NOT TARGET imgui-extension)
    set(IMGUI_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(IMGUI_DEMO OFF CACHE BOOL "" FORCE)
    set(IMGUI_ENABLE_FREETYPE OFF CACHE BOOL "" FORCE)
    set(IMGUI_IMPL_GLFW OFF CACHE BOOL "" FORCE)
    set(IMGUI_IMPL_VULKAN OFF CACHE BOOL "" FORCE)
    set(IMGUI_ENABLE_STDLIB_SUPPORT OFF CACHE BOOL "" FORCE)
    set(IMGUI_EXTENSION ON CACHE BOOL "" FORCE)

    add_subdirectory(vendor/imgui)
    target_link_libraries(${PLUGIN_NAME} PUBLIC imgui-extension)
endif ()

target_include_directories(${PLUGIN_NAME} PRIVATE include)
set_target_properties(${PLUGIN_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_DIR}"
)
#
#function(generate_info_xml)
#    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/info.xml.in" "${PROJECT_DIR}/info.xml" @ONLY)
#endfunction()

# Command to configure the file
set(GENERATED_INFO_XML "${PROJECT_DIR}/info.xml")
add_custom_command(
        OUTPUT ${GENERATED_INFO_XML}
        COMMAND ${CMAKE_COMMAND}
        -DIN_FILE= "${CMAKE_CURRENT_SOURCE_DIR}/info.xml.in"
        -DOUT_FILE= "${PROJECT_DIR}/info.xml"
        -P cmake/ProjectInfo.cmake
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/info.xml.in"
        COMMENT "Generating info.xml"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
# Ensure the custom command is executed every build
add_custom_target(GenerateInfoXml ALL DEPENDS ${GENERATED_INFO_XML})
add_dependencies(${PLUGIN_NAME} GenerateInfoXml)