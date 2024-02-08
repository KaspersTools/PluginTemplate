#
##disable cache
#set(IN_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/info.xml.in" CACHE STRING "The IN_FILE of the plugin")
#set(OUT_FILE "${PROJECT_BUILD_DIR}/info.xml" CACHE STRING "The OUT_FILE of the plugin")

# ConfigureFile.cmake
message(STATUS "Configuring ${IN_FILE} to ${OUT_FILE}")
configure_file(${IN_FILE} ${OUT_FILE} @ONLY)