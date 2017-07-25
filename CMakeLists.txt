cmake_minimum_required(VERSION 2.8)
project(ibcons)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(IBCONS_SOURCES
  src/main.c)

set(IBCONS_HEADERS
  )

add_executable(ibcons ${IBCONS_SOURCES} ${IBCONS_HEADERS})
target_link_libraries(ibcons ncursesw)
