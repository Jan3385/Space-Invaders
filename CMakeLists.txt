cmake_minimum_required(VERSION 3.12)
project(space-invaders C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")

add_executable(space-invaders
  ${PROJECT_SOURCES}
)

find_package(SDL2 REQUIRED)
target_link_libraries(space-invaders PRIVATE SDL2 SDL2_image SDL2_ttf m)

target_include_directories(space-invaders PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/source
) # ----


target_include_directories(space-invaders PRIVATE 
  ${SDL2_INCLUDE_DIRS} 
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/source
)

set(TEXTURE_FOLDER "${CMAKE_SOURCE_DIR}/assets")

add_custom_target(copy_assets ALL
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURE_FOLDER} ${CMAKE_BINARY_DIR}/assets
)

add_dependencies(space-invaders copy_assets)