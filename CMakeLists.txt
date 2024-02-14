cmake_minimum_required(VERSION 3.24.4)
project(HUMMINGBIRD_PLUGIN_TEMPLATE)
set(CMAKE_CXX_STANDARD 23)

if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()
if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()
if (NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif ()
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23)
endif ()

#TEST TEXT FOR WORKFLOW TEST
if (NOT TARGET HBUI)
    message(STATUS "imgui-extension not included, including it now")
    add_subdirectory(vendor/imgui)
    add_subdirectory(vendor/imgui/HBUI)
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