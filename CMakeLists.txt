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

# ---- Platform detection. ----
if(WIN32)
    message(STATUS "Building for Windows")
    set(PLATFORM_WINDOWS TRUE)
elseif(APPLE)
    message(STATUS "Building for macOS")
    set(PLATFORM_MACOS TRUE)
elseif(UNIX AND NOT APPLE)
    message(STATUS "Building for Linux")
    set(PLATFORM_LINUX TRUE)
endif()

# ---- Git Versioning. ----
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")

    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_LAST_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    execute_process(
        COMMAND ${GIT_EXECUTABLE} diff --quiet --exit-code
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_HAS_CHANGES
    )
else()
    set(GIT_LAST_TAG "v${PROJECT_VERSION}")
    set(GIT_COMMIT_HASH "unknown")
    set(GIT_HAS_CHANGES 0)
endif()

# ---- Build Information. ----
string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S" UTC)
string(TIMESTAMP BUILD_DATE_SHORT "%Y-%m-%d" UTC)

# ---- Form Full Version. ----
if(GIT_HAS_CHANGES EQUAL 0)
    set(BUILD_STATUS "clean")
else()
    set(BUILD_STATUS "dirty")
endif()

set(
    VERSION_STRING
    "${GIT_LAST_TAG}-${GIT_COMMIT_HASH}-${BUILD_STATUS}"
)

#---- Version Configuration. ----
configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/version.h.in"
    "${CMAKE_BINARY_DIR}/include/version.h"
)

#---- Source Files. ----
file(GLOB SOURCES "source/*.c")
file(GLOB HEADERS "include/*.h")

#---- Target Executable Files. ----
add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
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
if(PLATFORM_WINDOWS)
    # Windows dependencies.
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        winmm
        kernel32
        user32
        gdi32
    )
elseif(PLATFORM_LINUX)
    # Linux dependencies.
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
elseif(PLATFORM_MACOS)
    # macOS dependencies.
    find_library(COCOA_LIB Cocoa)
    find_library(IOKIT_LIB IOKit)
    find_library(OPENGL_LIB OpenGL)
    
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        ${COCOA_LIB}
        ${IOKIT_LIB}
        ${OPENGL_LIB}
    )
endif()

#---- Configuration information. ----
message(STATUS "Configuring ${PROJECT_NAME} ${VERSION_STRING}")
message(STATUS "Date: ${BUILD_DATE}")
message(STATUS "Commit hash: ${GIT_COMMIT_HASH}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
