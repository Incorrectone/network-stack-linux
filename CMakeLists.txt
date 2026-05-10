cmake_minimum_required(VERSION 3.12)

project(custom_stack)

set(HEADER_DIR include)
file(GLOB headers CONFIGURE_DEPENDS "${HEADER_DIR}/*.h")
file(GLOB sources CONFIGURE_DEPENDS "src/*.c")

add_library(custom STATIC ${sources} ${headers})

target_compile_options(custom PRIVATE -Wall -Wextra)

target_include_directories(custom PUBLIC "${HEADER_DIR}")

add_executable(${CMAKE_PROJECT_NAME} main.c)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra)

target_link_libraries(${CMAKE_PROJECT_NAME} custom)
