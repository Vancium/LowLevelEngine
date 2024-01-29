
include_directories("${PROJECT_SOURCE_DIR}/Engine")

set(
    SANDBOX_SOURCE_FILES
    src/Entry.c
    src/Game.c
)

add_executable(SandboxApp ${SANDBOX_SOURCE_FILES})

target_link_libraries(SandboxApp PUBLIC LLE_Engine)
