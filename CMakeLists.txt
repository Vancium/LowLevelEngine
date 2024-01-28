cmake_minimum_required(VERSION 3.5)

project(LLE_Sandbox)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Sandbox CACHE STRING "" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Engine CACHE STRING "" )

add_subdirectory(Engine)
add_subdirectory(Sandbox)

