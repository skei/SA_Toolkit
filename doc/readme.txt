----------------------------------------------------------------------
history
----------------------------------------------------------------------

* 0.4.0 (7.feb.2023)
  - clap 1.1.7

* 0.4.1
  - base: improved logging
  - clap: improved extensions handling
  - exe: creates plugin via clap_entry
  - gui: improved opengl handling
  - gui: buffered painting

----------------------------------------------------------------------
bugs
----------------------------------------------------------------------

*

----------------------------------------------------------------------
issues
----------------------------------------------------------------------

*

----------------------------------------------------------------------
todo
----------------------------------------------------------------------

  ----- linux -----

  * mouse cursors

  ----- win32 -----

  * 

  ----- clap -----

  * SAT_Win32Window
  * SAT_Win32OpenGL

  ----- wrappers -----

  * vst2
  * vst3
  * (ladspa)

----------------------------------------------------------------------
compiling
----------------------------------------------------------------------

  ----- linux -----

  * external libraries:

    pthread, rt, dl
    x11: X11, X11-xcb
    xcb: xcb, xcb-util, xcb-image, xcb-cursor, xcb-keysyms, xkbcommon
    gl: GL, GLX, GLU

  * compile (g++)

    debug:
      -std=c++17
      -fPIC
      -g
      -DSAT_PLUGIN -DSAT_DEBUG
      -I../src
      -c plugin.cpp
      -o plugin.o
      //-rdynamic"
      //-Wl,-Bsymbolic"
      //-static"

    release:
  	  -O3
  	  -fPIC
  	  -pthread
  	  -ffast-math

  * link (g++)

    debug:
      -shared
      plugin.o
      -o plugin.clap
      -static-libstdc++ -static-libgcc
      -fPIE
      -lpthread -lrt -ldl
      -lX11 -lX11-xcb
      -lxcb -lxcb-util -lxcb-image -lxcb-cursor -lxcb-keysyms -lxkbcommon
      -lGL -lGLX -lGLU

    release:
      -s

  ----- win32 -----

  * external libraries:

    (gdi32, msimg32, opengl32)

