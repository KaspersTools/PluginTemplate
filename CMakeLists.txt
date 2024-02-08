cmake_minimum_required(VERSION 3.24.4)
project(HUMMINGBIRD_PLUGIN_BASE)
set(CMAKE_CXX_STANDARD 23)
#TEST TEXT FOR WORKFLOW TEST
if (NOT TARGET imgui-extension)
    message(STATUS "imgui-extension not included, including it now")
    set(IMGUI_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(IMGUI_DEMO OFF CACHE BOOL "" FORCE)
    set(IMGUI_ENABLE_FREETYPE OFF CACHE BOOL "" FORCE)
    set(IMGUI_IMPL_GLFW OFF CACHE BOOL "" FORCE)
    set(IMGUI_IMPL_VULKAN OFF CACHE BOOL "" FORCE)
    set(IMGUI_ENABLE_STDLIB_SUPPORT OFF CACHE BOOL "" FORCE)
    set(IMGUI_EXTENSION ON CACHE BOOL "" FORCE)
    add_subdirectory(vendor/imgui)
else ()
    message(STATUS "imgui-extension already included")
endif ()

option(HUMMINGBIRD_BUILD_TEMPLATE "Build the plugin template" ON)

if (HUMMINGBIRD_BUILD_TEMPLATE)
    set(PLUGIN_NAME "PluginTemplate")
    set(PLUGIN_VERSION "1.0.0")
    set(PLUGIN_DESCRIPTION "This is the plugin template for Hummingbird")
    set(AUTHOR_NAME "Kasper de Bruin")
    set(AUTHOR_EMAIL "k.debruin@hellebrekers.nl")
    set(AUTHOR_WEBSITE "http://www.yourwebsite.com")
    set(PROJECT_BUILD_DIR  "${CMAKE_SOURCE_DIR}/build/plugins/${PLUGIN_NAME}")
    set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/PluginTemplate")

    include(PluginTemplate/template.cmake)

    # Path to the output info.xml file
    set(GENERATED_INFO_XML ${PROJECT_BUILD_DIR}/info.xml)
    message(STATUS "Generated info.xml: ${GENERATED_INFO_XML}")
    set(BASE_INFO_XML ${CMAKE_SOURCE_DIR}/info.xml.in)
    message(STATUS "Base info.xml: ${BASE_INFO_XML}")

    #IMPORTANT: This is the command that will generate the info.xml file
    # Make Sure To Not Put Space etc. in the COMMAND
    add_custom_command(
            OUTPUT ${GENERATED_INFO_XML}
            COMMAND ${CMAKE_COMMAND}
            -DPLUGIN_NAME="${PLUGIN_NAME}"
            -DPLUGIN_VERSION="${PLUGIN_VERSION}"
            -DPLUGIN_DESCRIPTION="${PLUGIN_DESCRIPTION}"
            -DAUTHOR_NAME="${AUTHOR_NAME}"
            -DAUTHOR_EMAIL="${AUTHOR_EMAIL}"
            -DAUTHOR_WEBSITE="${AUTHOR_WEBSITE}"
            -DIN_FILE="${BASE_INFO_XML}"
            -DOUT_FILE="${GENERATED_INFO_XML}"
            -P cmake/ProjectInfo.cmake
            DEPENDS ${BASE_INFO_XML}
            COMMENT "Generating info.xml for ${PLUGIN_NAME}"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    add_custom_target(GenerateInfoXml ALL DEPENDS ${GENERATED_INFO_XML})
    add_dependencies(${PLUGIN_NAME} GenerateInfoXml)
endif ()