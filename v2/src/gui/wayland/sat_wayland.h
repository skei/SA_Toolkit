#ifndef sat_wayland_included
#define sat_wayland_included
//----------------------------------------------------------------------

// https://jan.newmarch.name/Wayland/index.html
// https://jan.newmarch.name/Wayland/ProgrammingClient/

// Wayland EGL MUST be included before EGL headers

#include "sat.h"

#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <wayland-cursor.h>
#include <wayland-egl.h> 
#include <wayland-util.h>

#include "extern/wayland/xdg-shell.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
  #include "extern/wayland/xdg-shell.c.h"
#pragma GCC diagnostic pop



//----------------------------------------------------------------------
#endif
