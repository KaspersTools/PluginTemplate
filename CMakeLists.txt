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
        PluginSysInfo/src/SystemInfo.cpp
        PluginSysInfo/src/SystemInfo.h
        PluginSysInfo/src/Utils/Utils.h
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

# get Security Framework mac
find_library(SECURITY_FRAMEWORK Security)
if (NOT SECURITY_FRAMEWORK)
    message(FATAL_ERROR "Security not found Are you on mac?")
else ()
    message(STATUS "Security found")
    target_link_libraries(${PLUGIN_NAME} PUBLIC
            ${SECURITY_FRAMEWORK}
    )

    list(APPEND HUMMINGBIRD_LIBRARIES
            ${SECURITY_FRAMEWORK}
    )
endif ()

# get IOKit mac
find_library(IOKit IOKit)
if (NOT IOKit)
    message(FATAL_ERROR "IOKit not found Are you on mac?")
else ()
    message(STATUS "IOKit found")
    target_link_libraries(${PLUGIN_NAME} PUBLIC
            ${IOKit}
    )

    list(APPEND HUMMINGBIRD_LIBRARIES
            ${IOKit}
    )
endif ()


# get CoreFoundation mac
find_library(CoreFoundation CoreFoundation)
if (NOT CoreFoundation)
    message(FATAL_ERROR "CoreFoundation not found Are you on mac?")
else ()
    message(STATUS "CoreFoundation found")
    target_link_libraries(${PLUGIN_NAME} PUBLIC
            ${CoreFoundation}
    )

    list(APPEND HUMMINGBIRD_LIBRARIES
            ${CoreFoundation}
    )
endif ()


# Find the System Configuration framework on the system
find_library(SYSTEM_CONFIGURATION_FRAMEWORK SystemConfiguration)
if (NOT SYSTEM_CONFIGURATION_FRAMEWORK)
    message(FATAL_ERROR "SystemConfiguration not found Are you on mac?")
else ()
    message(STATUS "SystemConfiguration found")

    target_link_libraries(${PLUGIN_NAME} PUBLIC
            ${SYSTEM_CONFIGURATION_FRAMEWORK})

    list(APPEND HUMMINGBIRD_LIBRARIES ${SYSTEM_CONFIGURATION_FRAMEWORK})
endif ()