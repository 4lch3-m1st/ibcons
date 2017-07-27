ibcons
======

A quick IB browser. Highly experimental.

## Scratch & Design
Say we have a window as specified below, dimensions 100x27.
Right side of pane is left open for editing purposes
```
+==============================================================================================+
|                     [board1 / board2 / board3 ] [board4 / board5 / board6]                   |
|==============================================================================================|
| /board1/ - BOARD1                                                                            |
|+--------------------------------------------------------------------------------------------+|
||{SUBJECT - }Name, Time, No. no - [S][L][C]                                                  ||
|| +---------------+ comment comment comment comment comment comment comment comment          ||
|| |               |                                                                          ||
|| |               |                                                                          ||
|| |     IMAGE     |                                                                          ||
|| |               |                                                                          ||
|| |               |                                                                          ||
|| |               |                                                                          ||
|| +---------------+                                                                          ||
|+--------------------------------------------------------------------------------------------+|
|+--------------------------------------------------------------------------------------------+|
||{SUBJECT - }Name, Time, No. no - [S][L][C]                                                  ||
|| +---------------+ comment comment comment comment comment comment comment comment          ||
|| |               |                                                                          ||
|| |               |                                                                          ||
|| |     IMAGE     |                                                                          ||
|| |               |                                                                          ||
|| |               |                                                                          ||
|| |               |                                                                          ||
|| +---------------+                                                                          ||
|==============================================================================================|
| >                                                                                            |
+==============================================================================================+

{} = if existing
S, L, C = sticky, locked, cyclic
```


## Build
```bash
mkdir build && cd build
cmake ..
make```


## Dependencies
- ncurses
- panel for ncurses
- libcaca (images to ascii; soon)
- jansson
- curl

