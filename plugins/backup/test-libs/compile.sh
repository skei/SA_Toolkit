#!/bin/bash

g++ build.cpp -o build \
  -std=c++11 -Wl,--as-needed -fvisibility=hidden \
  -lpthread \
  -lX11 -lX11-xcb \
  -lxcb -lxcb-util -lxcb-image -lxcb-cursor -lxcb-keysyms -lxkbcommon \
  -lwayland-client -lwayland-egl \
  -lEGL \
  -lGL

# -lwayland-server -lwayland-client-protocol 
# -lGLESv2
# -lGL, -lGLX, -lGLU


