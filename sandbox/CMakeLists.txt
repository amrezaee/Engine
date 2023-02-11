set(SOURCE_LIST "Sandbox.hpp" "Sandbox.cpp")

add_executable(sandbox ${SOURCE_LIST})

target_link_libraries(sandbox PRIVATE core)