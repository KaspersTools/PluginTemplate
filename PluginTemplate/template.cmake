add_library(${PLUGIN_NAME} SHARED
        ${PROJECT_SOURCE_DIR}/PluginTemplate/src/PluginTemplate.cpp
        ${PROJECT_SOURCE_DIR}/PluginTemplate/src/PluginTemplate.h
        ${PROJECT_SOURCE_DIR}/PluginTemplate/src/PluginTemplateWindow.h
)


if(TARGET HBUI)
    target_link_libraries(${PLUGIN_NAME} PUBLIC HBUI)
else()
    message(FATAL_ERROR "HBUI not found")
endif()

target_include_directories(${PLUGIN_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(${PLUGIN_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BUILD_DIR}"
)
