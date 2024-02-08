set(PLUGIN_NAME "HUMMINGBIRDPLUGIN")
set(PLUGIN_VERSION "1.0.0")
set(PLUGIN_DESCRIPTION "This is the plugin template for Hummingbird")
set(AUTHOR_NAME "Kasper de Bruin")
set(AUTHOR_EMAIL "k.debruin@hellebrekers.nl")
set(AUTHOR_WEBSITE "http://www.yourwebsite.com")

set(PROJECT_DIR ${HUMMINGBIRD_PLUGIN_${PLUGIN_NAME}_DIR})
if (NOT DEFINED HUMMINGBIRD_PLUGIN_DIR)
    set(PROJECT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME})
else ()
    set(PROJECT_DIR ${HUMMINGBIRD_PLUGIN_DIR}/${PLUGIN_NAME})
endif ()

#disable cache
set(IN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/info.xml.in" CACHE STRING "The IN_FILE of the plugin")
set(OUT_FILE "${PROJECT_DIR}/info.xml" CACHE STRING "The OUT_FILE of the plugin")
# ConfigureFile.cmake
configure_file(${IN_FILE} ${OUT_FILE} @ONLY)