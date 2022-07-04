cmake_minimum_required(VERSION 3.22)
set(PROJECT GameBloat)
project(${PROJECT})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

add_definitions("-Wall" "-g")
include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE BASE_SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
list(FILTER BASE_SOURCES EXCLUDE REGEX "main_.*\\.c$")

# Desktop GTK application config
set(DESKTOP_SOURCES "src/Platforms/main_gtk.c")
list(APPEND DESKTOP_SOURCES ${BASE_SOURCES})

add_executable(Desktop ${DESKTOP_SOURCES})
target_link_libraries(Desktop ${GTK4_LIBRARIES})
target_include_directories(Desktop PUBLIC ${GTK4_INCLUDE_DIRS})

set (SERVER_SOURCES "src/Platforms/main_server.c")
list(APPEND SERVER_SOURCES ${BASE_SOURCES})
add_executable(Server ${SERVER_SOURCES})
target_include_directories(Server PUBLIC ${PROJECT_SOURCE_DIR}/libs/wsServer/include)
target_link_libraries(Server ${PROJECT_SOURCE_DIR}/libs/wsServer/libws.a)
