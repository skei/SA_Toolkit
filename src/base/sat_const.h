#ifndef sat_const_included
#define sat_const_included
//----------------------------------------------------------------------

#define SAT_VERSION                 "0.4.0"
#define SAT_VENDOR                  "skei.audio"
#define SAT_URL                     "https://github.com/skei/SA_Toolkit"

#define SAT_MAX_NAME_LENGTH         256
#define SAT_MAX_PATH_LENGTH         512

const char  SAT_HEX_TABLE[]   = "0123456789ABCDEF";
const char  SAT_CSYMBOLS[]    = "________________________________________________0123456789_______ABCDEFGHIJKLMNOPQRSTUVWXYZ______abcdefghijklmnopqrstuvwxyz_____";
const char  SAT_NULL_STRING[] = "";

// 4           4  5         5
// 123456789abcdef0123456789a
// abcdefghijklmnopqrstuvwxyz
// 5f = _

//#define SAT_MAGIC       0x535f0000 // 'M_'   // + 0x0000
//#define SAT_MAGIC_USER  0x6d5f0000 // 'm_'   // + 0x0000
//#define SAT_MAGIC_M_PL  0x535f504c // M_PL   // plugin
//#define SAT_MAGIC_M_ED  0x4d5f4544 // M_ED   // editor

#define SAT_MAGIC                   0x5341545f  // 'SAT_'
#define SAT_MAGIC_PLUGIN            0x53415450  // 'SATP'
#define SAT_MAGIC_EDITOR            0x53415445  // 'SATE'
#define SAT_MAGIC_USER              0x7361745f  // 'sat_'

//#ifndef SAT_CPP11
//  #define nullptr 0
//#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define SAT_AUDIO_FILE_READ         0x10
#define SAT_AUDIO_FILE_WRITE        0x20
#define SAT_AUDIO_FILE_RDWR         0x30

#define MIP_BUTTON_NONE             0
#define MIP_BUTTON_LEFT             1
#define MIP_BUTTON_MIDDLE           2
#define MIP_BUTTON_RIGHT            3
#define MIP_BUTTON_SCROLL_UP        4
#define MIP_BUTTON_SCROLL_DOWN      5
#define MIP_BUTTON_BACKWARD         8
#define MIP_BUTTON_FORWARD          9

#define SAT_FILE_READ_BINARY        (char*)"rb"
#define SAT_FILE_READ_TEXT          (char*)"r"
#define SAT_FILE_WRITE_BINARY       (char*)"wb"
#define SAT_FILE_WRITE_TEXT         (char*)"w"
#define SAT_FILE_APPEND_BINARY      (char*)"ab"
#define SAT_FILE_APPEND_TEXT        (char*)"a"

#define SAT_KEY_NONE                0x00
#define SAT_KEY_SHIFT               0x01
#define SAT_KEY_CAPS                0x02
#define SAT_KEY_CTRL                0x04
#define SAT_KEY_ALT                 0x08
#define SAT_KEY_ALTGR               0x10

#define SAT_KEY_ENTER               13

#define SAT_KEY_ENTER               13
#define SAT_KEY_ESC                 27
#define SAT_KEY_HOME                36
#define SAT_KEY_END                 35
#define SAT_KEY_LEFT                37
#define SAT_KEY_RIGHT               39
#define SAT_KEY_DELETE              46
#define SAT_KEY_BACKSPACE           8

#define SAT_MIDI_NOTE_OFF           0x80
#define SAT_MIDI_NOTE_ON            0x90
#define SAT_MIDI_POLY_AFTERTOUCH    0xA0
#define SAT_MIDI_CONTROL_CHANGE     0xB0
#define SAT_MIDI_PROGRAM_CHANGE     0xC0
#define SAT_MIDI_CHANNEL_AFTERTOUCH 0xD0
#define SAT_MIDI_PITCHBEND          0xE0
#define SAT_MIDI_SYS                0xF0

#define SAT_OPENGL_MAJOR            3
#define SAT_OPENGL_MINOR            2

#define SAT_PLUGIN_MAX_PARAMETERS   1024

#define SAT_WINDOW_THREAD_ID_KILL          666

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define SAT_PI              3.1415926535
#define SAT_PI2             (SAT_PI * 2.0)
#define SAT_PI05            (SAT_PI * 0.5)
#define SAT_INVPI           (1.0 / SAT_PI)
#define SAT_INVPI2          (1.0 / SAT_PI2)
#define SAT_INVPI05         (1.0 / SAT_PI05)

#define SAT_TINY            0.001
#define SAT_VERY_TINY       0.00001

#define SAT_INV360          (1.0 / 360.0)
#define SAT_INV12           (1.0 / 12.0)
#define SAT_INV16           (1.0 / 16.0)
#define SAT_INV32           (1.0 / 32.0)
#define SAT_INV127          (1.0 / 127.0)
#define SAT_INV255          (1.0 / 255.0)
#define SAT_INV256          ( 1.0 / 256.0)
#define SAT_INV8192         (1.0 / 8192.0)

#define SAT_SQRT2           sqrt(2.0f)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define SAT_CHAR_BIT        8
#define SAT_CHAR_MAX        0x7f                /* 127 */
#define SAT_SHORT_MAX       0x7fff              /* 32767 */
#define SAT_INT32_MAX       0x7fffffff          /* 2147483647 */
#define SAT_UINT32_MAX      0xffffffff          /* 8589934591U */
#define SAT_INT64_MAX       0x7fffffffffffffff  /* 9223372036854775807LL */
#define SAT_UINT64_MAX      0x7fffffffffffffff  /* 18446744073709551615ULL */
#define SAT_FLOAT_MAX       3.40282347e+38F
#define SAT_FLOAT_MIN       1.17549435E-38F
#define SAT_FLOAT_EPSILON   1.19209290E-7F
#define SAT_FLOAT_DENORM    1.40129846E-45F
#define SAT_DOUBLE_MAX      1.7976931348623157e+308
#define SAT_DOUBLE_MIN      2.2250738585072014E-308
#define SAT_DOUBLE_EPSILON  2.2204460492503131E-16
#define SAT_DOUBLE_DENORM   4.9406564584124654E-324

#ifdef SAT_32BIT
  #define SAT_INT_MAX       SAT_INT32_MAX   // 2147483647L
  #define SAT_UINT_MAX      SAT_UINT32_MAX  // 8589934591UL
#endif

#ifdef SAT_64BIT
  #define SAT_INT_MAX      SAT_INT64_MAX   // 9223372036854775807L
  #define SAT_UINT_MAX     SAT_UINT64_MAX  // 18446744073709551615UL
#endif

//#define SAT_RANDMAX        RAND_MAX  // 2147483647
#define SAT_RANDMAX        0x7fffffff
#define SAT_INVRANDMAX     (1.0 / (double)SAT_RANDMAX)

//----------------------------------------------------------------------
// enums
//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif
