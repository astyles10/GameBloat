cmake_minimum_required(VERSION 3.22)
set(PROJECT GameBloat)
project(${PROJECT})

# Replace requirement with disallowing Desktop build
find_package(PkgConfig REQUIRED)
# pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

# Replace requirement with disallowing Server build
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_definitions("-Wall" "-g3" "-fstack-protector-strong" "-Ofast") #"-Wshadow" "-Wpointer-arith" "-Wcast-qual" "-Wstrict-prototypes" "-Wmissing-prototypes")

if (DEFINED DEBUG_MEMORY)
  add_definitions("-fsanitize=address")
  add_link_options("-fsanitize=address")
endif()
include(TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if (NOT IS_BIG_ENDIAN)
  message(STATUS "Setting LITTLE_ENDIAN!")
  add_compile_definitions(LITTLE_ENDIAN LIN)
endif(NOT IS_BIG_ENDIAN)

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE BASE_SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
list(FILTER BASE_SOURCES EXCLUDE REGEX "main_.*\\.c$")

file(GLOB_RECURSE CINOOP_SOURCES ${PROJECT_SOURCE_DIR}/Cinoop/source/*.c)
list(FILTER BASE_SOURCES EXCLUDE REGEX "main.c$")
# add_compile_definitions(LIN)

list(APPEND BASE_SOURCES "libs/cJSON/cJSON.c")

# ##################################################### #
#                   Debug configuration                 #
# ##################################################### #

set(DEBUG_SOURCES "src/Platforms/main_debug_testing.c")
list(APPEND DEBUG_SOURCES ${BASE_SOURCES})
list(APPEND DEBUG_SOURCES ${CINOOP_SOURCES})

add_executable(Debug ${DEBUG_SOURCES})
target_include_directories(Debug PUBLIC ${PROJECT_SOURCE_DIR}/libs/cJSON ${PROJECT_SOURCE_DIR}/Cinoop/include)
target_link_libraries(Debug PUBLIC -lX11 -lGL -lGLU -lm)
# target_compile_definitions(Debug PUBLIC "DEBUG_PRINT")
# target_compile_options(Debug PUBLIC)
