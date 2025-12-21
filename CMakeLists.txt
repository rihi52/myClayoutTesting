cmake_minimum_required(VERSION 3.27)
project(guidnbatter)
set(CMAKE_C_STANDARD 99)

# === OUTPUT DIRECTORIES ===
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# === SOURCE FILES ===
set(SRC_FILES
    src/main.c
    src/main_window.c
    src/styles.c
    src/global.c
    SDL3/clay_renderer_SDL3.c
    src/db_query.c
    src/sql/sqlite3.c
    src/build_encounter.c
    src/creature_db.c
    src/start_encounter.c
    src/slider.c
)

# === SDL3 (vendored) ===
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# === SDL3_ttf (vendored) ===
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# === SDL3_image (vendored) ===
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

# === BUILD TARGET ===
add_executable(${PROJECT_NAME} ${SRC_FILES})

# === INCLUDE DIRECTORIES ===
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/sql   # ✅ SQLite headers
)

# === LINK LIBRARIES ===
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
    m            # ✅ math library (needed by SQLite on some systems)
    pthread      # ✅ threading library (needed by SQLite)
    dl           # ✅ dynamic loading (needed by SQLite)
)

# === RESOURCE COPY ===
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
)

# === IDE GROUPING (optional) ===
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Apps")


##### NATIVE WINDOWS VERSION (comment out above for this) #####

# cmake_minimum_required(VERSION 3.27)
# project(guidnbatter)
# set(CMAKE_C_STANDARD 99)

# # === OUTPUT DIRECTORIES ===
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# # === SDL3 OPTIONS (before add_subdirectory) ===
# # Tell SDL_ttf to use vendored FreeType instead of system FreeType
# set(SDL3TTF_VENDORED ON CACHE BOOL "" FORCE)
# set(SDL3TTF_FREETYPE_VENDORED ON CACHE BOOL "" FORCE)

# # Disable other optional dependencies that might cause issues
# set(SDL3IMAGE_VENDORED ON CACHE BOOL "" FORCE)

# # === SOURCE FILES ===
# set(SRC_FILES
#     src/main.c
#     src/main_window.c
#     src/styles.c
#     src/global.c
#     SDL3/clay_renderer_SDL3.c
#     src/db_query.c
#     src/sql/sqlite3.c
#     src/build_encounter.c
#     src/creature_db.c
#     src/start_encounter.c
# )

# # === SDL3 (vendored) ===
# add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# # === SDL3_ttf (vendored) ===
# add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# # === SDL3_image (vendored) ===
# add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

# # === BUILD TARGET ===
# add_executable(${PROJECT_NAME} ${SRC_FILES})

# # === INCLUDE DIRECTORIES ===
# target_include_directories(${PROJECT_NAME} PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_SOURCE_DIR}/src/sql   # ✅ SQLite headers
# )

# # === LINK LIBRARIES ===
# target_link_libraries(${PROJECT_NAME} PRIVATE
#     SDL3::SDL3
#     SDL3_ttf::SDL3_ttf
#     SDL3_image::SDL3_image
# )

# # Platform-specific libraries
# if(UNIX AND NOT APPLE)
#     # Linux-specific libraries
#     target_link_libraries(${PROJECT_NAME} PRIVATE
#         m            # math library
#         pthread      # threading library
#         dl           # dynamic loading
#     )
# elseif(WIN32)
#     # Windows doesn't need m, pthread, dl - they're built-in or use Windows API
#     # Uncomment if you need Windows-specific libraries:
#     # target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 winmm)
# endif()

# # === RESOURCE COPY ===
# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_CURRENT_SOURCE_DIR}/resources
#         $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
# )

# # === IDE GROUPING (optional) ===
# set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Apps")