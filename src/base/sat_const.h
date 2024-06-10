#ifndef sat_const_included
#define sat_const_included
//----------------------------------------------------------------------

#define SAT_MAGIC                               0x5341545f  // 'SAT_'
#define SAT_MAGIC_USER                          0x7361745f  // 'sat_'
#define SAT_MAGIC_PLUGIN                        0x53415450  // 'SATP'
#define SAT_MAGIC_EDITOR                        0x53415445  // 'SATE'

#define SAT_EVENT_SPACE_ID                      SAT_MAGIC // CLAP_CORE_EVENT_SPACE_ID

//----------

const char  SAT_NULL_STRING[]   = "";
const char  SAT_HEX_TABLE[]     = "0123456789ABCDEF";
const char  SAT_CSYMBOLS[]      = "________________________________"
                                "________________0123456789______"
                                "_ABCDEFGHIJKLMNOPQRSTUVWXYZ_____"
                                "_abcdefghijklmnopqrstuvwxyz_____";
const char* SAT_MONTH_NAMES[13] = { "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec", "errorember" };

//----------------------------------------------------------------------
// std
//----------------------------------------------------------------------

#define SAT_PI                                  3.14159265358979323846
#define SAT_PI2                                 (SAT_PI * 2.0)
#define SAT_PI05                                (SAT_PI * 0.5)
#define SAT_INVPI                               (1.0 / SAT_PI)
#define SAT_INVPI2                              (1.0 / SAT_PI2)
#define SAT_INVPI05                             (1.0 / SAT_PI05)

#define SAT_TINY                                0.001
#define SAT_VERY_TINY                           0.00001

#define SAT_INV360                              (1.0 / 360.0)
#define SAT_INV12                               (1.0 / 12.0)
#define SAT_INV16                               (1.0 / 16.0)
#define SAT_INV32                               (1.0 / 32.0)
#define SAT_INV127                              (1.0 / 127.0)
#define SAT_INV255                              (1.0 / 255.0)
#define SAT_INV256                              (1.0 / 256.0)
#define SAT_INV8192                             (1.0 / 8192.0)

#define SAT_SQRT2                               sqrt(2.0)

//----------

#define SAT_CHAR_BIT                            8
#define SAT_CHAR_MAX                            0x7f                      // 127
#define SAT_SHORT_MAX                           0x7fff                    // 32767
#define SAT_INT32_MAX                           0x7fffffff                // 2147483647
#define SAT_UINT32_MAX                          0xffffffff                // 8589934591U
#define SAT_INT64_MAX                           0x7fffffffffffffff        // 9223372036854775807LL
#define SAT_UINT64_MAX                          0x7fffffffffffffff        // 18446744073709551615ULL
#define SAT_FLOAT_MAX                           3.40282347e+38F
#define SAT_FLOAT_MIN                           1.17549435E-38F
#define SAT_FLOAT_EPSILON                       1.19209290E-7F
#define SAT_FLOAT_DENORM                        1.40129846E-45F
#define SAT_DOUBLE_MAX                          1.7976931348623157e+308
#define SAT_DOUBLE_MIN                          2.2250738585072014E-308
#define SAT_DOUBLE_EPSILON                      2.2204460492503131E-16
#define SAT_DOUBLE_DENORM                       4.9406564584124654E-324

#if defined (SAT_32BIT)
#define SAT_INT_MAX                             SAT_INT32_MAX             // 2147483647L
#define SAT_UINT_MAX                            SAT_UINT32_MAX            // 8589934591UL
#endif

#if defined (SAT_64BIT)
#define SAT_INT_MAX                             SAT_INT64_MAX             // 9223372036854775807L
#define SAT_UINT_MAX                            SAT_UINT64_MAX            // 18446744073709551615UL
#endif

#define SAT_RANDMAX                             0x7fffffff
#define SAT_INVRANDMAX                          (1.0 / (double)SAT_RANDMAX)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define SAT_BUTTON_NONE                         0
#ifdef SAT_LINUX
  #define SAT_BUTTON_LEFT                       1
  #define SAT_BUTTON_MIDDLE                     2
  #define SAT_BUTTON_RIGHT                      3
  #define SAT_BUTTON_SCROLL_UP                  4
  #define SAT_BUTTON_SCROLL_DOWN                5
  #define SAT_BUTTON_SIDE_BACKWARD              8
  #define SAT_BUTTON_SIDE_FORWARD               9
#endif

#define SAT_CORNER_NONE                         0
#define SAT_CORNER_UPPER_LEFT                   0x01
#define SAT_CORNER_UPPER_RIGHT                  0x02
#define SAT_CORNER_LOWER_RIGHT                  0x04
#define SAT_CORNER_LOWER_LEFT                   0x08

#define SAT_CORNER_UPPER                        (SAT_CORNER_UPPER_LEFT  | SAT_CORNER_UPPER_RIGHT)
#define SAT_CORNER_LOWER                        (SAT_CORNER_LOWER_LEFT  | SAT_CORNER_LOWER_RIGHT)
#define SAT_CORNER_LEFT                         (SAT_CORNER_UPPER_LEFT  | SAT_CORNER_LOWER_LEFT)
#define SAT_CORNER_RIGHT                        (SAT_CORNER_UPPER_RIGHT | SAT_CORNER_LOWER_RIGHT)
#define SAT_CORNER_ALL                          (SAT_CORNER_UPPER       | SAT_CORNER_LOWER)

#define SAT_CURSOR_LOCK                        -4
#define SAT_CURSOR_UNLOCK                      -3
#define SAT_CURSOR_SHOW                        -2
#define SAT_CURSOR_HIDE                        -1
#define SAT_CURSOR_DEFAULT                      0
#define SAT_CURSOR_ARROW                        1
#define SAT_CURSOR_ARROW_UP                     2
#define SAT_CURSOR_ARROW_DOWN                   3
#define SAT_CURSOR_ARROW_LEFT                   4
#define SAT_CURSOR_ARROW_RIGHT                  5
#define SAT_CURSOR_ARROW_UP_DOWN                6
#define SAT_CURSOR_ARROW_LEFT_RIGHT             7
#define SAT_CURSOR_ARROW_TOP_LEFT               8
#define SAT_CURSOR_ARROW_TOP_RIGHT              9
#define SAT_CURSOR_ARROW_BOTTOM_LEFT            10
#define SAT_CURSOR_ARROW_BOTTOM_RIGHT           11
#define SAT_CURSOR_ARROW_LEFT_SIDE              12
#define SAT_CURSOR_ARROW_RIGHT_SIDE             13
#define SAT_CURSOR_ARROW_TOP_SIDE               14
#define SAT_CURSOR_ARROW_BOTTOM_SIDE            15
#define SAT_CURSOR_MOVE                         16
#define SAT_CURSOR_WAIT                         17
#define SAT_CURSOR_ARROW_WAIT                   18 // !!!
#define SAT_CURSOR_HAND                         19
#define SAT_CURSOR_FINGER                       20
#define SAT_CURSOR_CROSS                        21
#define SAT_CURSOR_CROSS2                       22
#define SAT_CURSOR_PENCIL                       23
#define SAT_CURSOR_PLUS                         24
#define SAT_CURSOR_QUESTION                     25
#define SAT_CURSOR_IBEAM                        26
#define SAT_CURSOR_ARROW_INVALID                27
#define SAT_CURSOR_INVALID                      28
#define SAT_CURSOR_X                            29
#define SAT_CURSOR_COUNT                        30

#define SAT_DIRECTION_NONE                      0x00
#define SAT_DIRECTION_LEFT                      0x01
#define SAT_DIRECTION_RIGHT                     0x02
#define SAT_DIRECTION_UP                        0x04
#define SAT_DIRECTION_DOWN                      0x08
#define SAT_DIRECTION_TOP                       0x04
#define SAT_DIRECTION_BOTTOM                    0x08
#define SAT_DIRECTION_HORIZ                     (SAT_DIRECTION_LEFT  + SAT_DIRECTION_RIGHT)
#define SAT_DIRECTION_VERT                      (SAT_DIRECTION_UP    + SAT_DIRECTION_DOWN)
#define SAT_DIRECTION_ALL                       (SAT_DIRECTION_HORIZ + SAT_DIRECTION_VERT)

#define SAT_EASING_NONE           1
#define SAT_EASING_LINEAR         1
#define SAT_EASING_IN_QUAD        2
#define SAT_EASING_OUT_QUAD       3
#define SAT_EASING_INOUT_QUAD     4
#define SAT_EASING_OUTIN_QUAD     5
#define SAT_EASING_IN_CUBIC       6
#define SAT_EASING_OUT_CUBIC      7
#define SAT_EASING_INOUT_CUBIC    8
#define SAT_EASING_OUTIN_CUBIC    9
#define SAT_EASING_IN_QUART       10
#define SAT_EASING_OUT_QUART      11
#define SAT_EASING_INOUT_QUART    12
#define SAT_EASING_OUTIN_QUART    13
#define SAT_EASING_IN_QUINT       14
#define SAT_EASING_OUT_QUINT      15
#define SAT_EASING_INOUT_QUINT    16
#define SAT_EASING_OUTIN_QUINT    17
#define SAT_EASING_IN_SINE        18
#define SAT_EASING_OUT_SINE       19
#define SAT_EASING_INOUT_SINE     20
#define SAT_EASING_OUTIN_SINE     21
#define SAT_EASING_IN_EXPO        22
#define SAT_EASING_OUT_EXPO       23
#define SAT_EASING_INOUT_EXPO     24
#define SAT_EASING_OUTIN_EXPO     25
#define SAT_EASING_IN_CIRC        26
#define SAT_EASING_OUT_CIRC       27
#define SAT_EASING_INOUT_CIRC     28
#define SAT_EASING_OUTIN_CIRC     29
#define SAT_EASING_IN_ELASTIC     30
#define SAT_EASING_OUT_ELASTIC    31
#define SAT_EASING_INOUT_ELASTIC  32
#define SAT_EASING_OUTIN_ELASTIC  33
#define SAT_EASING_IN_BACK        34
#define SAT_EASING_OUT_BACK       35
#define SAT_EASING_INOUT_BACK     36
#define SAT_EASING_OUTIN_BACK     37
#define SAT_EASING_OUT_BOUNCE     38
#define SAT_EASING_IN_BOUNCE      39
#define SAT_EASING_INOUT_BOUNCE   40
#define SAT_EASING_OUTIN_BOUNCE   41


#define SAT_EDGE_NONE                           0
#define SAT_EDGE_LEFT                           0x01
#define SAT_EDGE_RIGHT                          0x02
#define SAT_EDGE_TOP                            0x04
#define SAT_EDGE_BOTTOM                         0x08
#define SAT_EDGE_ALL                            0x0F
#define SAT_EDGE_HORIZ                          (SAT_EDGE_LEFT | SAT_EDGE_RIGHT)
#define SAT_EDGE_VERT                           (SAT_EDGE_TOP | SAT_EDGE_BOTTOM)

#define SAT_FILE_READ_BINARY                    (char*)"rb"
#define SAT_FILE_READ_TEXT                      (char*)"r"
#define SAT_FILE_WRITE_BINARY                   (char*)"wb"
#define SAT_FILE_WRITE_TEXT                     (char*)"w"
#define SAT_FILE_APPEND_BINARY                  (char*)"ab"
#define SAT_FILE_APPEND_TEXT                    (char*)"a"

#define SAT_GUI_TYPE_NONE                       0
#define SAT_GUI_TYPE_WAYLAND                    1
#define SAT_GUI_TYPE_WIN32                      2
#define SAT_GUI_TYPE_X11                        3

#define SAT_GUI_TYPE_NAME_NONE                  "None"
#define SAT_GUI_TYPE_NAME_WAYLAND               "Wayland"
#define SAT_GUI_TYPE_NAME_WIN32                 "Win32"
#define SAT_GUI_TYPE_NAME_X11                   "X11"

#define SAT_KEY_NONE                            0
#define SAT_KEY_ENTER                           13
#define SAT_KEY_ESC                             27
#define SAT_KEY_SPACE                           32
#define SAT_KEY_HOME                            36
#define SAT_KEY_END                             35
#define SAT_KEY_LEFT                            37
#define SAT_KEY_RIGHT                           39
#define SAT_KEY_DELETE                          46
#define SAT_KEY_BACKSPACE                       8

#define SAT_MIDI_NOTE_OFF                       0x80
#define SAT_MIDI_NOTE_ON                        0x90
#define SAT_MIDI_POLY_AFTERTOUCH                0xA0
#define SAT_MIDI_CONTROL_CHANGE                 0xB0
#define SAT_MIDI_PROGRAM_CHANGE                 0xC0
#define SAT_MIDI_CHANNEL_AFTERTOUCH             0xD0
#define SAT_MIDI_PITCHBEND                      0xE0
#define SAT_MIDI_SYS                            0xF0

#define SAT_OBSERVE_NONE                        0
#define SAT_OBSERVE_DOUBLE                      1

#define SAT_PAINTER_TYPE_NONE                   0
#define SAT_PAINTER_TYPE_CAIRO                  1
#define SAT_PAINTER_TYPE_NANOVG                 2
#define SAT_PAINTER_TYPE_WIN32                  3
#define SAT_PAINTER_TYPE_X11                    4

#define SAT_PAINTER_TYPE_NAME_NONE              "None"
#define SAT_PAINTER_TYPE_NAME_CAIRO             "Cairo"
#define SAT_PAINTER_TYPE_NAME_NANOVG            "NanoVG"
#define SAT_PAINTER_TYPE_NAME_WIN32             "Win32"
#define SAT_PAINTER_TYPE_NAME_X11               "X11"

#define SAT_PLUGIN_BLOCK_PROCESSOR              0
#define SAT_PLUGIN_INTERLEAVED_PROCESSOR        1
#define SAT_PLUGIN_QUANTIZED_PROCESSOR          2
#define SAT_PLUGIN_VOICE_PROCESSOR              3

#define SAT_PLUGIN_FORMAT_NONE                  0
#define SAT_PLUGIN_FORMAT_CLAP                  1
#define SAT_PLUGIN_FORMAT_DSSI                  2
#define SAT_PLUGIN_FORMAT_EXE                   3
#define SAT_PLUGIN_FORMAT_LADSPA                4
#define SAT_PLUGIN_FORMAT_LV2                   5
#define SAT_PLUGIN_FORMAT_VST2                  6
#define SAT_PLUGIN_FORMAT_VST3                  7

#define SAT_PLUGIN_FORMAT_NONE_NAME             "NONE"
#define SAT_PLUGIN_FORMAT_CLAP_NAME             "CLAP"
#define SAT_PLUGIN_FORMAT_DSSI_NAME             "DSSI"
#define SAT_PLUGIN_FORMAT_EXE_NAME              "EXE"
#define SAT_PLUGIN_FORMAT_LADSPA_NAME           "LADSPA"
#define SAT_PLUGIN_FORMAT_LV2_NAME              "LV2"
#define SAT_PLUGIN_FORMAT_VST2_NAME             "VST2"
#define SAT_PLUGIN_FORMAT_VST3_NAME             "VST3"

#define SAT_RENDERER_TYPE_NONE                  0
#define SAT_RENDERER_TYPE_GLX                   1
#define SAT_RENDERER_TYPE_EGL                   2
#define SAT_RENDERER_TYPE_WGL                   3
#define SAT_RENDERER_TYPE_WIN32                 4
#define SAT_RENDERER_TYPE_X11                   5

#define SAT_PAINTER_TYPE_NAME_NONE               "None"
#define SAT_PAINTER_TYPE_NAME_EGL                "EGL"
#define SAT_PAINTER_TYPE_NAME_GLX                "GLX"
#define SAT_PAINTER_TYPE_NAME_WGL                "WGL"
#define SAT_PAINTER_TYPE_NAME_WIN32              "Win32"
#define SAT_PAINTER_TYPE_NAME_X11                "X11"

#define SAT_STATE_NONE                          0
#ifdef SAT_LINUX
  #define SAT_STATE_SHIFT                       0x01
  #define SAT_STATE_CAPS                        0x02
  #define SAT_STATE_CTRL                        0x04
  #define SAT_STATE_ALT                         0x08
  #define SAT_STATE_ALTGR                       0x10
#endif

#define SAT_SYMBOL_NONE                         0
#define SAT_SYMBOL_RECT                         1
#define SAT_SYMBOL_CIRCLE                       2
#define SAT_SYMBOL_TRI_UP                       3
#define SAT_SYMBOL_TRI_DOWN                     4
#define SAT_SYMBOL_TRI_LEFT                     5
#define SAT_SYMBOL_TRI_RIGHT                    6
#define SAT_SYMBOL_FILLED_RECT                  7
#define SAT_SYMBOL_FILLED_CIRCLE                8
#define SAT_SYMBOL_FILLED_TRI_UP                9
#define SAT_SYMBOL_FILLED_TRI_DOWN              10
#define SAT_SYMBOL_FILLED_TRI_LEFT              11
#define SAT_SYMBOL_FILLED_TRI_RIGHT             12
#define SAT_SYMBOL_SINE                         13
#define SAT_SYMBOL_SAW                          14
#define SAT_SYMBOL_SQUARE                       15
#define SAT_SYMBOL_TRI                          16
#define SAT_SYMBOL_NOISE                        17

#define SAT_TEXT_ALIGN_CENTER                   0
#define SAT_TEXT_ALIGN_LEFT                     0x01
#define SAT_TEXT_ALIGN_RIGHT                    0x02
#define SAT_TEXT_ALIGN_TOP                      0x04
#define SAT_TEXT_ALIGN_BOTTOM                   0x08

#define SAT_TWEEN_VALUE                         0
#define SAT_TWEEN_RECT                          1
// #define SAT_TWEEN_CALLBACK                   1
// #define SAT_TWEEN_PAUSE                      2
// #define SAT_TWEEN_BG_COLOR                   4
// #define SAT_TWEEN_TXT_COLOR                  5
// #define SAT_TWEEN_TRANS                      6

#define SAT_VOICE_OFF                           0
#define SAT_VOICE_WAITING                       1
#define SAT_VOICE_PLAYING                       2
#define SAT_VOICE_RELEASED                      3
#define SAT_VOICE_FINISHED                      4

#define SAT_WIDGET_REALIGN_POS                  0
#define SAT_WIDGET_REALIGN_ORDER                1

#define SAT_WIDGET_REDRAW_ALL                   0
#define SAT_WIDGET_REDRAW_PARAM                 1
#define SAT_WIDGET_REDRAW_MOD                   2

#define SAT_WIDGET_UPDATE_VALUE                 0

#define SAT_WINDOW_TYPE_NONE                    0
#define SAT_WINDOW_TYPE_WAYLAND                 1
#define SAT_WINDOW_TYPE_WIN32                   2
#define SAT_WINDOW_TYPE_X11                     3

#define SAT_WINDOW_TYPE_NAME_NONE               "None"
#define SAT_WINDOW_TYPE_NAME_WAYLAND            "Wayland"
#define SAT_WINDOW_TYPE_NAME_WIN32              "Win32"
#define SAT_WINDOW_TYPE_NAME_X11                "X11"

//----------------------------------------------------------------------
// widget layout
//----------------------------------------------------------------------

#define SAT_WIDGET_LAYOUT_DEFAULT               0

#define SAT_WIDGET_LAYOUT_ANCHOR_LEFT           0x000001
#define SAT_WIDGET_LAYOUT_ANCHOR_RIGHT          0x000002
#define SAT_WIDGET_LAYOUT_ANCHOR_TOP            0x000004
#define SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM         0x000008

#define SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT       (SAT_WIDGET_LAYOUT_ANCHOR_TOP + SAT_WIDGET_LAYOUT_ANCHOR_LEFT)
#define SAT_WIDGET_LAYOUT_ANCHOR_TOP_RIGHT      (SAT_WIDGET_LAYOUT_ANCHOR_TOP + SAT_WIDGET_LAYOUT_ANCHOR_RIGHT)
#define SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT    (SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM + SAT_WIDGET_LAYOUT_ANCHOR_LEFT)
#define SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_RIGHT   (SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM + SAT_WIDGET_LAYOUT_ANCHOR_RIGHT)

#define SAT_WIDGET_LAYOUT_ANCHOR_TOP_CENTER     (SAT_WIDGET_LAYOUT_ANCHOR_TOP + SAT_WIDGET_LAYOUT_ANCHOR_CENTER_HORIZ)
#define SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_CENTER  (SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM + SAT_WIDGET_LAYOUT_ANCHOR_CENTER_HORIZ)
#define SAT_WIDGET_LAYOUT_ANCHOR_LEFT_CENTER    (SAT_WIDGET_LAYOUT_ANCHOR_LEFT + SAT_WIDGET_LAYOUT_ANCHOR_CENTER_VERT)
#define SAT_WIDGET_LAYOUT_ANCHOR_RIGHT_CENTER   (SAT_WIDGET_LAYOUT_ANCHOR_RIGHT + SAT_WIDGET_LAYOUT_ANCHOR_CENTER_VERT)

#define SAT_WIDGET_LAYOUT_ANCHOR_CENTER_HORIZ   0x000010
#define SAT_WIDGET_LAYOUT_ANCHOR_CENTER_VERT    0x000020
#define SAT_WIDGET_LAYOUT_ANCHOR_CENTER         (SAT_WIDGET_LAYOUT_ANCHOR_CENTER_HORIZ + SAT_WIDGET_LAYOUT_ANCHOR_CENTER_VERT)

#define SAT_WIDGET_LAYOUT_STRETCH_LEFT          0x000100
#define SAT_WIDGET_LAYOUT_STRETCH_RIGHT         0x000200
#define SAT_WIDGET_LAYOUT_STRETCH_TOP           0x000400
#define SAT_WIDGET_LAYOUT_STRETCH_BOTTOM        0x000800
#define SAT_WIDGET_LAYOUT_STRETCH_HORIZ         (SAT_WIDGET_LAYOUT_STRETCH_LEFT + SAT_WIDGET_LAYOUT_STRETCH_RIGHT)
#define SAT_WIDGET_LAYOUT_STRETCH_VERT          (SAT_WIDGET_LAYOUT_STRETCH_TOP + SAT_WIDGET_LAYOUT_STRETCH_BOTTOM)
#define SAT_WIDGET_LAYOUT_STRETCH_ALL           (SAT_WIDGET_LAYOUT_STRETCH_HORIZ + SAT_WIDGET_LAYOUT_STRETCH_VERT)

#define SAT_WIDGET_LAYOUT_FILL_LEFT             0x001000
#define SAT_WIDGET_LAYOUT_FILL_RIGHT            0x002000
#define SAT_WIDGET_LAYOUT_FILL_TOP              0x004000
#define SAT_WIDGET_LAYOUT_FILL_BOTTOM           0x008000

#define SAT_WIDGET_LAYOUT_PERCENT               0x010000
#define SAT_WIDGET_LAYOUT_PERCENT_PARENT        0x020000
#define SAT_WIDGET_LAYOUT_PERCENT_LAYOUT        0x040000

#define SAT_WIDGET_LAYOUT_TOP_LEFT              ( SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT       | SAT_WIDGET_LAYOUT_FILL_TOP)
#define SAT_WIDGET_LAYOUT_TOP_CENTER            ( SAT_WIDGET_LAYOUT_ANCHOR_TOP_CENTER     | SAT_WIDGET_LAYOUT_FILL_TOP)
#define SAT_WIDGET_LAYOUT_TOP_RIGHT             ( SAT_WIDGET_LAYOUT_ANCHOR_TOP_RIGHT      | SAT_WIDGET_LAYOUT_FILL_TOP)

#define SAT_WIDGET_LAYOUT_BOTTOM_LEFT           ( SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT    | SAT_WIDGET_LAYOUT_FILL_BOTTOM)
#define SAT_WIDGET_LAYOUT_BOTTOM_CENTER         ( SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_CENTER  | SAT_WIDGET_LAYOUT_FILL_BOTTOM)
#define SAT_WIDGET_LAYOUT_BOTTOM_RIGHT          ( SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_RIGHT   | SAT_WIDGET_LAYOUT_FILL_BOTTOM)

#define SAT_WIDGET_LAYOUT_LEFT_TOP              ( SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT       | SAT_WIDGET_LAYOUT_FILL_LEFT)
#define SAT_WIDGET_LAYOUT_LEFT_CENTER           ( SAT_WIDGET_LAYOUT_ANCHOR_LEFT_CENTER    | SAT_WIDGET_LAYOUT_FILL_LEFT)
#define SAT_WIDGET_LAYOUT_LEFT_BOTTOM           ( SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT    | SAT_WIDGET_LAYOUT_FILL_LEFT)

#define SAT_WIDGET_LAYOUT_RIGHT_TOP             ( SAT_WIDGET_LAYOUT_ANCHOR_TOP_RIGHT      | SAT_WIDGET_LAYOUT_FILL_RIGHT)
#define SAT_WIDGET_LAYOUT_RIGHT_CENTER          ( SAT_WIDGET_LAYOUT_ANCHOR_RIGHT_CENTER   | SAT_WIDGET_LAYOUT_FILL_RIGHT)
#define SAT_WIDGET_LAYOUT_RIGHT_BOTTOM          ( SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_RIGHT   | SAT_WIDGET_LAYOUT_FILL_RIGHT)

//----------------------------------------------------------------------
// terminal
//----------------------------------------------------------------------

// https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124
// 0 Reset/Normal, 1 Bold, 2 Faint, 3 Italics, 4 Underlined
// "\e[1;97m" (bold white)

#if defined (SAT_LINUX)

  #define SAT_TERM_BG_BLACK                     "\e[40m"
  #define SAT_TERM_BG_RED                       "\e[41m"
  #define SAT_TERM_BG_GREEN                     "\e[42m"
  #define SAT_TERM_BG_YELLOW                    "\e[43m"
  #define SAT_TERM_BG_BLUE                      "\e[44m"
  #define SAT_TERM_BG_MAGENTA                   "\e[45m"
  #define SAT_TERM_BG_CYAN                      "\e[46m"
  #define SAT_TERM_BG_LIGHT_GREY                "\e[47m"

  #define SAT_TERM_BG_GREY                      "\e[100m"
  #define SAT_TERM_BG_LIGHT_RED                 "\e[101m"
  #define SAT_TERM_BG_LIGHT_GREEN               "\e[102m"
  #define SAT_TERM_BG_LIGHT_YELLOW              "\e[103m"
  #define SAT_TERM_BG_LIGHT_BLUE                "\e[104m"
  #define SAT_TERM_BG_LIGHT_MAGENTA             "\e[105m"
  #define SAT_TERM_BG_LIGHT_CYAN                "\e[106m"
  #define SAT_TERM_BG_WHITE                     "\e[107m"

  #define SAT_TERM_FG_BLACK                     "\e[30m"
  #define SAT_TERM_FG_RED                       "\e[31m"
  #define SAT_TERM_FG_GREEN                     "\e[32m"
  #define SAT_TERM_FG_YELLOW                    "\e[33m"
  #define SAT_TERM_FG_BLUE                      "\e[34m"
  #define SAT_TERM_FG_MAGENTA                   "\e[35m"
  #define SAT_TERM_FG_CYAN                      "\e[36m"
  #define SAT_TERM_FG_LIGHT_GREY                "\e[37m"

  #define SAT_TERM_FG_GREY                      "\e[90m"
  #define SAT_TERM_FG_LIGHT_RED                 "\e[91m"
  #define SAT_TERM_FG_LIGHT_GREEN               "\e[92m"
  #define SAT_TERM_FG_LIGHT_YELLOW              "\e[93m"
  #define SAT_TERM_FG_LIGHT_BLUE                "\e[94m"
  #define SAT_TERM_FG_LIGHT_MAGENTA             "\e[95m"
  #define SAT_TERM_FG_LIGHT_CYAN                "\e[96m"
  #define SAT_TERM_FG_WHITE                     "\e[97m"

  #define SAT_TERM_RESET                        "\e[0m"
  
  #define SAT_TERM_NORMAL                       "\e[0;97m"
  #define SAT_TERM_BOLD                         "\e[1;97m"
  #define SAT_TERM_FAINT                        "\e[2;97m"
  #define SAT_TERM_ITALICS                      "\e[3;97m"
  #define SAT_TERM_UNDERLINE                    "\e[4;97m"
  
#else // WIN32

  #define SAT_TERM_BG_BLACK                     ""
  #define SAT_TERM_BG_RED                       ""
  #define SAT_TERM_BG_GREEN                     ""
  #define SAT_TERM_BG_YELLOW                    ""
  #define SAT_TERM_BG_BLUE                      ""
  #define SAT_TERM_BG_MAGENTA                   ""
  #define SAT_TERM_BG_CYAN                      ""
  #define SAT_TERM_BG_LIGHT_GREY                ""

  #define SAT_TERM_BG_GREY                      ""
  #define SAT_TERM_BG_LIGHT_RED                 ""
  #define SAT_TERM_BG_LIGHT_GREEN               ""
  #define SAT_TERM_BG_LIGHT_YELLOW              ""
  #define SAT_TERM_BG_LIGHT_BLUE                ""
  #define SAT_TERM_BG_LIGHT_MAGENTA             ""
  #define SAT_TERM_BG_LIGHT_CYAN                ""
  #define SAT_TERM_BG_WHITE                     ""

  #define SAT_TERM_FG_BLACK                     ""
  #define SAT_TERM_FG_RED                       ""
  #define SAT_TERM_FG_GREEN                     ""
  #define SAT_TERM_FG_YELLOW                    ""
  #define SAT_TERM_FG_BLUE                      ""
  #define SAT_TERM_FG_MAGENTA                   ""
  #define SAT_TERM_FG_CYAN                      ""
  #define SAT_TERM_FG_LIGHT_GREY                ""

  #define SAT_TERM_FG_GREY                      ""
  #define SAT_TERM_FG_LIGHT_RED                 ""
  #define SAT_TERM_FG_LIGHT_GREEN               ""
  #define SAT_TERM_FG_LIGHT_YELLOW              ""
  #define SAT_TERM_FG_LIGHT_BLUE                ""
  #define SAT_TERM_FG_LIGHT_MAGENTA             ""
  #define SAT_TERM_FG_LIGHT_CYAN                ""
  #define SAT_TERM_FG_WHITE                     ""

  #define SAT_TERM_RESET                        ""

  #define SAT_TERM_NORMAL                       ""
  #define SAT_TERM_BOLD                         ""
  #define SAT_TERM_FAINT                        ""
  #define SAT_TERM_ITALICS                      ""
  #define SAT_TERM_UNDERLINE                    ""

#endif

//----------------------------------------------------------------------
#endif