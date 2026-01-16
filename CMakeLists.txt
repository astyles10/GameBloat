cmake_minimum_required(VERSION 3.22)
set(PROJECT GameBloat)
project(${PROJECT})

# Replace requirement with disallowing Desktop build
find_package(PkgConfig REQUIRED)
# pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

# Replace requirement with disallowing Server build
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_definitions("-Wall" "-g3" "-fstack-protector-strong" "-Wshadow" "-Wpointer-arith" "-Wcast-qual" "-Wstrict-prototypes" "-Wmissing-prototypes")

if (DEFINED DEBUG_MEMORY) 
  add_definitions("-fsanitize=address")
  add_link_options("-fsanitize=address")
endif()
include(TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if (NOT IS_BIG_ENDIAN)
  message(STATUS "Setting LITTLE_ENDIAN!")
  add_compile_definitions(LITTLE_ENDIAN)
endif(NOT IS_BIG_ENDIAN)

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE BASE_SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
list(FILTER BASE_SOURCES EXCLUDE REGEX "main_.*\\.c$")
list(APPEND BASE_SOURCES "libs/cJSON/cJSON.c")

# ##################################################### #
#         Desktop GTK application configuration         #
# ##################################################### #

set(DESKTOP_SOURCES "src/Platforms/main_gtk.c")
list(APPEND DESKTOP_SOURCES ${BASE_SOURCES})

add_executable(Desktop ${DESKTOP_SOURCES})
target_link_libraries(Desktop ${GTK4_LIBRARIES})
target_include_directories(Desktop PUBLIC ${GTK4_INCLUDE_DIRS})

# ##################################################### #
#            Server application configuration           #
# ##################################################### #

set(SERVER_SOURCES "src/Platforms/main_server.c")
list(APPEND SERVER_SOURCES ${BASE_SOURCES})

add_executable(Server ${SERVER_SOURCES})
target_include_directories(Server PUBLIC ${PROJECT_SOURCE_DIR}/libs/wsServer/include ${PROJECT_SOURCE_DIR}/libs/cJSON)
target_link_libraries(Server ${PROJECT_SOURCE_DIR}/libs/wsServer/libws.a Threads::Threads)

# ##################################################### #
#                   Debug configuration                 #
# ##################################################### #

set(DEBUG_SOURCES "src/Platforms/main_debug.c")
list(APPEND DEBUG_SOURCES ${BASE_SOURCES})

add_executable(Debug ${DEBUG_SOURCES})
target_include_directories(Debug PUBLIC ${PROJECT_SOURCE_DIR}/libs/cJSON)
# target_compile_definitions(Debug PUBLIC "DEBUG_PRINT")
target_compile_options(Debug PUBLIC)
