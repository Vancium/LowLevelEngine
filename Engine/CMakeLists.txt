

set(
    LLE_SOURCE_FILES
    src/core/Test/test.c
    src/core/Test/test.h
    src/defines.h
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_library(LLE_Engine SHARED ${LLE_SOURCE_FILES})

add_compile_definitions(LLE_EXPORT=1)

target_include_directories(LLE_Engine PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src")

