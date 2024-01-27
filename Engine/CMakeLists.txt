

set(
    LLE_SOURCE_FILES
    src/core/Log/log.c
    src/core/Log/log.h
    src/core/Assert/assertions.h
    src/defines.h
    
)



set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(-DLLE_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(-DLLE_DEBUG)
endif()

add_library(LLE_Engine SHARED ${LLE_SOURCE_FILES})

add_compile_definitions(LLE_EXPORT=1)

target_include_directories(LLE_Engine PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src")



