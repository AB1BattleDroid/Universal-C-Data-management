cmake_minimum_required(VERSION 3.10)
project(UVCDLibrary)

# Enable position-independent code for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define the library sources
set(SOURCES uvcd.c)
set(HEADERS uvcd.h)

# Create the static library
add_library(uvcd_static STATIC ${SOURCES})
set_target_properties(uvcd_static PROPERTIES OUTPUT_NAME "uvcd")

# Create the shared library
add_library(uvcd_shared SHARED ${SOURCES})
set_target_properties(uvcd_shared PROPERTIES OUTPUT_NAME "uvcd")

# Install rules (optional)
install(TARGETS uvcd_static uvcd_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS} DESTINATION include)

