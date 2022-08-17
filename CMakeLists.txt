cmake_minimum_required(VERSION 3.9)

project(
  DiscordVcTts
  VERSION 0.1
  LANGUAGES C)

set(REPO_URL "https://github.com/djpiper28/DiscordVcTts")

message(STATUS "Building ${CMAKE_PROJECT_NAME}")
message(STATUS "Git repo: ${REPO_URL}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "Build OS: ${CMAKE_SYSTEM_NAME}")

# Get the branch name
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Commit info: ${GIT_COMMIT_HASH}@${GIT_BRANCH}")

# Version info
add_definitions("-DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")
add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")
add_definitions(
  "-DVERSION=\"${CMAKE_PROJECT_VERSION} (${GIT_COMMIT_HASH}@${GIT_BRANCH})\"")
add_definitions("-DOS=\"${CMAKE_SYSTEM_NAME}\"")
add_definitions("-DREPO_URL=\"${REPO_URL}\"")
add_definitions("-DBUFFER_LENGTH=4096")
add_definitions("-DPROJECT_VERSION=\"${CMAKE_PROJECT_VERSION}\"")
add_definitions("-DPROJECT_NAME=\"${CMAKE_PROJECT_NAME}\"")

# __FILENAME__
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")
add_definitions("-D__FILENAME__=(__FILE__ + SOURCE_PATH_SIZE)")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Non-ui files that are tested
set(MAIN_FILES
    ./src/utils.c
    ./src/utils.h
    ./src/tts_queue.c
    ./src/tts_queue.h
    ./testing.h/logger.c
    ./testing.h/logger.h
    ./testing.h/ansi_colour.h)

set(TEST_FILES
    ${MAIN_FILES}
    ./tests/test_utils.c
    ./tests/test_utils.h
    ./testing.h/logger.c
    ./testing.h/logger.h
    ./testing.h/testing.c
    ./testing.h/testing.h
    ./testing.h/ansi_colour.h)

set(LIBERALS pthread curl discord jansson)

add_executable(DiscordVcTts ${MAIN_FILES} ./src/main.c)
target_link_libraries(DiscordVcTts ${LIBERALS})

add_executable(DiscordVcTtsTests ${TEST_FILES} ./tests/main.c)
target_link_libraries(DiscordVcTtsTests ${LIBERALS})

file(COPY mem_tests.sh DESTINATION ${CMAKE_BINARY_DIR})
file(COPY mem_tests.py DESTINATION ${CMAKE_BINARY_DIR})

enable_testing()
add_test(NAME main-tests COMMAND SquireDesktopTests)
add_test(NAME memory-tests COMMAND bash mem_tests.sh)
