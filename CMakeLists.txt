cmake_minimum_required(VERSION 3.12)
project(space-invaders C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")

add_executable(space-invaders
  ${PROJECT_SOURCES}
)

#find_package(SDL2 REQUIRED)
if(WIN32) # ----
  include(FetchContent)

  set(SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE)
  set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)
  set(SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE)
  set(SDL2TTF_INSTALL OFF CACHE BOOL "" FORCE)
  set(SDL2_DISABLE_INSTALL ON CACHE BOOL "" FORCE)

  FetchContent_Declare(SDL2
    URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.9/SDL2-2.30.9.tar.gz)
  FetchContent_Declare(SDL2_image
    URL https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-2.8.2.tar.gz)
  FetchContent_Declare(SDL2_ttf
    URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-2.22.0.tar.gz)
  FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf)

  target_link_libraries(space-invaders PRIVATE
    SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

  # copy the SDL DLLs next to the exe
  add_custom_command(TARGET space-invaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:space-invaders> $<TARGET_RUNTIME_DLLS:space-invaders>
    COMMAND_EXPAND_LISTS)
else()
  find_package(SDL2 REQUIRED)
  target_include_directories(space-invaders PRIVATE ${SDL2_INCLUDE_DIRS})
  target_link_libraries(space-invaders PRIVATE SDL2 SDL2_image SDL2_ttf m)
endif()

target_include_directories(space-invaders PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/source
) # ----


target_include_directories(space-invaders PRIVATE 
  ${SDL2_INCLUDE_DIRS} 
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/source
)
#target_link_libraries(space-invaders SDL2 SDL2_image SDL2_ttf)

set(TEXTURE_FOLDER "${CMAKE_SOURCE_DIR}/assets")

add_custom_target(copy_assets ALL
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURE_FOLDER} ${CMAKE_BINARY_DIR}/assets
)

add_dependencies(space-invaders copy_assets)