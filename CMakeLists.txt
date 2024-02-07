cmake_minimum_required(VERSION 3.24.4)
project(HUMMINGBIRD_PLUGIN_SYSINFO)
set(CMAKE_CXX_STANDARD 23)

set(PLUGIN_NAME ${CMAKE_PROJECT_NAME})
set(PROJECT_DIR ${HUMMINGBIRD_PLUGIN_${PLUGIN_NAME}_DIR})

if (NOT DEFINED HUMMINGBIRD_PLUGIN_DIR)
    set(PROJECT_DIR ${CMAKE_CURRENT_BINARY_DIR})
else ()
    set(PROJECT_DIR ${HUMMINGBIRD_PLUGIN_DIR}/${CMAKE_PROJECT_NAME})
endif ()
message(STATUS "HUMMINGBIRD_PLUGIN_MANAGER_DIR: ${PROJECT_DIR}")

add_library(${PLUGIN_NAME} SHARED
        PluginSysInfo/src/PluginSysInfo.cpp
        PluginSysInfo/src/PluginSysInfo.h
        PluginSysInfo/src/UI/PluginSysInfoWindow.h
        PluginSysInfo/src/UI/PluginSysInfoWindow.cpp
)

if(NOT TARGET imgui-extension)
    set(IMGUI_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(IMGUI_DEMO OFF CACHE BOOL "" FORCE)
    set(IMGUI_ENABLE_FREETYPE OFF CACHE BOOL "" FORCE)
    set(IMGUI_IMPL_GLFW OFF CACHE BOOL "" FORCE)
    set(IMGUI_IMPL_VULKAN OFF CACHE BOOL "" FORCE)

    set(IMGUI_ENABLE_STDLIB_SUPPORT OFF CACHE BOOL "" FORCE)
    set(IMGUI_EXTENSION ON CACHE BOOL "" FORCE)

    add_subdirectory(vendor/imgui)
    target_link_libraries(${PLUGIN_NAME} PUBLIC imgui-extension)
endif()

target_include_directories(${PLUGIN_NAME} PRIVATE include)
set_target_properties(${PLUGIN_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_DIR}"
)
