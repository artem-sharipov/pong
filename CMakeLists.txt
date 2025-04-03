cmake_minimum_required(VERSION 3.14)
project(
    pong
    VERSION 0.1.0
    LANGUAGES C
)

# ---- General Settings. ----
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Raylib Dependencies
set(RAYLIB_VERSION 5.5)
# QUIET or REQUIRED
find_package(raylib ${RAYLIB_VERSION} QUIET)
# If there's none, fetch and build raylib
if (NOT raylib_FOUND) 
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  # Have we downloaded raylib yet?
  if (NOT raylib_POPULATED) 
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
    # don't build the supplied examples
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  endif()
endif()


# ---- Git Versioning. ----
set(GIT_LAST_TAG "v${PROJECT_VERSION}")
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")

    execute_process(
        COMMAND ${GIT_EXECUTABLE} tag v${PROJECT_VERSION}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        ERROR_QUIET
    )

    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

else()
    set(GIT_COMMIT_HASH "unknown")
endif()

# ---- Build Information. ----
string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S" UTC)
string(TIMESTAMP BUILD_DATE_SHORT "%Y-%m-%d" UTC)

set(
    VERSION_STRING
    "${GIT_LAST_TAG}-${GIT_COMMIT_HASH} (${BUILD_DATE})"
)

#---- Version Configuration. ----
configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/version.h.in"
    "${CMAKE_BINARY_DIR}/include/version.h"
)

#---- Source Files. ----
file(GLOB SOURCES "source/*.c")

#---- Target Executable Files. ----
add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

#---- Include Directories. ----
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_BINARY_DIR}/include"
)

#---- Link Common Libraries. ----
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    raylib
)

#---- Platform-Specific Dependencies. ----
if(WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        winmm
        kernel32
        user32
        gdi32
    )
elseif(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
    find_package(OpenGL REQUIRED)
    
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        X11
        m
        pthread
        dl
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENGL_LIBRARIES}
    )
endif()

#---- Configuration information. ----
message(STATUS "Configuring ${PROJECT_NAME} ${VERSION_STRING}")
message(STATUS "Date: ${BUILD_DATE}")
message(STATUS "Commit hash: ${GIT_COMMIT_HASH}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
