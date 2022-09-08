cmake_minimum_required(VERSION 3.15)
project(OGLCPP)

set(LAUNCHER_SRC_DIR launcher/)

file(GLOB_RECURSE LAUNCHER_SOURCE "${LAUNCHER_SRC_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${LAUNCHER_SOURCE})

add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)

include_directories(lib/glad/include)
include_directories(lib/glfw/include)
include_directories(lib/stb)
include_directories(lib/glm)

set(ENGINE_SRC_DIR engine/)
set(GLFW_BUILD_EXAMPLES OFF)

file(GLOB_RECURSE ENGINE_SOURCE "${ENGINE_SRC_DIR}/*.cpp" "${ENGINE_SRC_DIR}/*.h")


add_library(engine MODULE ${ENGINE_SOURCE})

set_target_properties(engine PROPERTIES PREFIX "")

target_link_libraries(engine PUBLIC glad)
target_link_libraries(engine PUBLIC glfw)

file(COPY res/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

