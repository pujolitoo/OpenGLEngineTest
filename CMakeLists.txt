cmake_minimum_required(VERSION 3.15)
project(OGLCPP)

set(LAUNCHER_SRC_DIR launcher/)
set(VENDOR_DIR vendor/)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB_RECURSE LAUNCHER_SOURCE "${LAUNCHER_SRC_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${LAUNCHER_SOURCE})

add_subdirectory(${VENDOR_DIR}/glad)
add_subdirectory(${VENDOR_DIR}/glfw)

include_directories(${VENDOR_DIR}/glad/include)
include_directories(${VENDOR_DIR}/glfw/include)
include_directories(${VENDOR_DIR}/stb)
include_directories(${VENDOR_DIR}/glm/glm)

set(ENGINE_SRC_DIR engine/)

file(GLOB_RECURSE ENGINE_SOURCE "${ENGINE_SRC_DIR}/*.cpp" "${ENGINE_SRC_DIR}/*.h")


add_library(engine MODULE ${ENGINE_SOURCE})

set_target_properties(engine PROPERTIES PREFIX "")

target_link_libraries(engine PUBLIC glad)
target_link_libraries(engine PUBLIC glfw)

file(COPY res/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

