cmake_minimum_required(VERSION 3.27)
project(clay_sdl3_demo C)
set(CMAKE_C_STANDARD 99)

# Source files
set(SRC_FILES
    src/main.c
    SDL3/clay_renderer_SDL3.c
)

# Fetch SDL3 and dependencies
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# SDL3
FetchContent_Declare(
  SDL
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.6
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL)
set_property(DIRECTORY "${sdl_SOURCE_DIR}" PROPERTY EXCLUDE_FROM_ALL TRUE)

# SDL3_ttf
FetchContent_Declare(
  SDL_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG release-3.2.2
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL_ttf)
set_property(DIRECTORY "${sdl_ttf_SOURCE_DIR}" PROPERTY EXCLUDE_FROM_ALL TRUE)

# SDL3_image
FetchContent_Declare(
  SDL_image
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL_image.git"
  GIT_TAG release-3.2.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL_image)
set_property(DIRECTORY "${SDL_image_SOURCE_DIR}" PROPERTY EXCLUDE_FROM_ALL TRUE)

# Build executable
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
    SDL3::SDL3
)

# Copy resources after build
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
)
