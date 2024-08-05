cmake_minimum_required(VERSION 3.5.0)
project(SA_Toolkit VERSION 3.0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(build
  build/build.cpp
)

target_include_directories(build PUBLIC
  "${PROJECT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}/src"
)

target_compile_definitions(build PUBLIC
  -DSAT_DEBUG
  -DSAT_PLUGIN_EXE
  -DSAT_GUI_X11
)

target_link_libraries(build
  pthread
  dl rt
  X11 X11-xcb
  xcb xcb-util xcb-image xcb-cursor xcb-keysyms
  xkbcommon
  GL GLX
)

#add_subdirectory(src/gui)
#add_subdirectory(src/plugin)
