
include_directories("${PROJECT_SOURCE_DIR}/Engine")
add_executable(SandboxApp src/main.c)

target_link_libraries(SandboxApp PUBLIC LLE_Engine)
