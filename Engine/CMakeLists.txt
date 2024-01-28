

set(
    LLE_SOURCE_FILES
    src/defines.h
    src/core/Log/log.c
    src/core/Log/log.h
    src/core/Assert/assertions.h
    src/platform/platform.h
    src/platform/platform_win32.c


    
)



set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(LLE_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(LLE_DEBUG)
endif()

if(WIN32)
    # find window specifc libraries
    find_library(USER32 user32)
    message("WINDOWS_____________________")
endif(WIN32)

add_library(LLE_Engine SHARED ${LLE_SOURCE_FILES})


add_compile_definitions(LLE_EXPORT=1)

target_include_directories(LLE_Engine PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src")

if(WIN32)
    target_link_libraries(LLE_Engine PUBLIC USER32)
endif(WIN32)



