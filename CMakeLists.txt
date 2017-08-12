cmake_minimum_required(VERSION 2.8)
project(ibcons)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(IBCONS_SOURCES
  src/main.c
  src/aesthetics.c
  src/winctrl.c
  src/postview.c
  src/networking.c)

set(IBCONS_HEADERS
  src/main.h
  src/aesthetics.h
  src/winctrl.h
  src/postview.h
  src/networking.h)

add_executable(ibcons ${IBCONS_SOURCES} ${IBCONS_HEADERS})
target_link_libraries(ibcons ncursesw panel jansson curl)
