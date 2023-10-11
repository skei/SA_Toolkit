#!/bin/bash

# work in progress!

# $PWD  = current path
# $0    = script name/executable
# $@    = arguments
# $#    = number of arguments?


#VAR_1="to uppercase"
#echo ${VAR_1^}
#echo ${VAR_1^^}

#VAR_2="TO  LOWERCASE!"
#echo ${VAR_2,}
#echo ${VAR_2,,}

#VAR_3="Reverse Cases"
#echo ${VAR_3~}
#echo ${VAR_3~~}

#----------------------------------------------------------------------
#
#
#
#----------------------------------------------------------------------

INPUT="build.cpp"
OUTPUT="build"
FORMAT="CLAP"
GUI="NANOVG"
DEBUG=false
TEST=false
LIBRARY=false

FLAGS=""
FLAGS+=" -std=gnu++17"
FLAGS+=" -Wall"
FLAGS+=" -Wno-unknown-pragmas -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -Wno-narrowing"
FLAGS+=" -pthread"
FLAGS+=" -Wl,--as-needed"
FLAGS+=" -static-libstdc++"
FLAGS+=" -static-libgcc"

INC=""
INC+=" -I../src"
#INC+=" -I../src/extern/vst3/vst3sdk"

LIB=""
LIB+=" -ldl -lrt"
LIB+=" -lX11 -lX11-xcb -lxcb -lxcb-util -lxcb-image -lxcb-cursor -lxkbcommon -lxcb-keysyms"
LIB+=" -lGL -lGLU -lGLX"
LIB+=" -lEGL"

#----------------------------------------------------------------------
#
# print help
#
#----------------------------------------------------------------------

function print_help_and_exit {
  echo "usage: ./compile.sh <options>"
  echo "  -i <path>   input file name/path"
  echo "  -o <path>   output file name/path"
  echo "  -f <type>   format (all,clap,dssi,exe,ladspa,lv2,vst2,vst3)"
  echo "  -g <type>   gui (cairo,gdi,nanovg,nogui,x11)"
  echo "  -d          debug mode"
  echo "  -t          test mode"
  echo "  -h, -?      this help"
  exit 1
}

#----------------------------------------------------------------------
#
# parse command line
#
#----------------------------------------------------------------------

if [ "$#" -eq 0 ]; then
  print_help_and_exit
fi

while getopts i:o:f:g:dth? arg
do
  case $arg in
    i)    INPUT=$OPTARG;;
    o)    OUTPUT=$OPTARG;;
    f)    FORMAT=${OPTARG^^};;
    g)    GUI=${OPTARG^^};;
    d)    DEBUG=true;;
    t)    TEST=true;;
    h|?)  print_help_and_exit;;
    *)    exit 1;;
  esac
done

#------------------------------
# -i input
#------------------------------

#if [ $INPUT = "" ]; then
#  INPUT="build.clap"
#fi

INPUT_FILE=${INPUT##*/}
INPUT_BASE=${INPUT_FILE%%.*}
INPUT_EXT=${INPUT_FILE##*.}
INPUT_DIR=${INPUT%$INPUT_FILE}

#echo ""
#echo "INPUT     : $INPUT"
#echo "INPUT_FILE: $INPUT_FILE"
#echo "INPUT_BASE: $INPUT_BASE"
#echo "INPUT_EXT : $INPUT_EXT"
#echo "INPUT_DIR : $INPUT_DIR"

#------------------------------
# -o output
#------------------------------

OUTPUT_FILE=${OUTPUT##*/}
OUTPUT_BASE=${OUTPUT_FILE%%.*}
OUTPUT_EXT=${OUTPUT_FILE##*.}
OUTPUT_DIR=${OUTPUT%$OUTPUT_FILE}

#echo "OUTPUT     : $OUTPUT"
#echo "OUTPUT_FILE: $OUTPUT_FILE"
#echo "OUTPUT_BASE: $OUTPUT_BASE"
#echo "OUTPUT_EXT : $OUTPUT_EXT"
#echo "OUTPUT_DIR : $OUTPUT_DIR" 

#if [ "$OUTPUT" = "" ]; then
#  OUTPUT=$INPUT_BASE
#fi

#------------------------------
# -f format
#------------------------------

case $FORMAT in
  "ALL")
    #echo "format = all"
    #OUTPUT+=".bin"
    LIBRARY=true
    FLAGS+=" -DSAT_PLUGIN_ALL"
    ;;
  "CLAP")
    #echo "format = clap"
    #OUTPUT+=".clap"
    LIBRARY=true
    FLAGS+=" -DSAT_PLUGIN_CLAP"
    ;;
  "DSSI")
    #echo "format = dssi"
    #OUTPUT+=".so"
    LIBRARY=true
    FLAGS+=" -DSAT_PLUGIN_DSSI"
    ;;
  "EXE")
    #echo "format = exe"
    #OUTPUT+=".exe"
    LIBRARY=false
    FLAGS+=" -DSAT_PLUGIN_EXE"
    ;;
  "LADSPA")
    #echo "format = ladspa"
    #OUTPUT+=".so"
    LIBRARY=true
    FLAGS+=" -DSAT_PLUGIN_LADSPA"
    ;;
  "LV2")
    #echo "format = lv2"
    #OUTPUT+=".so"
    LIBRARY=true
    FLAGS+=" -DSAT_PLUGIN_LV2"
    ;;
  "VST2")
    #echo "format = vst2"
    #OUTPUT+=".so"
    LIBRARY=true
    FLAGS+=" -DSAT_PLUGIN_VST2"
    ;;
  "VST3")
    #echo "format = vst3"
    #OUTPUT+=".so"
    LIBRARY=true
    FLAGS+=" -DSAT_PLUGIN_VST3"
    INC+=" -I../extern/vst3/vst3sdk"
    ;;
  *)
    echo "unknown format:" $FORMAT
    exit 1
    ;;
esac

#----------

#if [ "$FORMAT" = "" ]; then
#  FORMAT="CLAP"
#fi

if $LIBRARY
  then
    #echo "library"
    FLAGS+=" -shared"
    FLAGS+=" -fPIC"
    FLAGS+=" -DSAT_LIBRARY"
  else
    #echo "application"
    FLAGS+=" -DSAT_EXE"
fi


#------------------------------
# -g gui
#------------------------------

case $GUI in
  "CAIRO")
    #echo "gui = cairo"
    FLAGS+=" -DSAT_GUI_CAIRO"
    ;;
  "GDI")
    #echo "gui = gdi"
    FLAGS+=" -DSAT_GUI_GDI"
    ;;
  "NANOVG")
    #echo "gui = nanovg"
    FLAGS+=" -DSAT_GUI_NANOVG"
    ;; 
  "NOGUI")
    #echo "gui = nogui"
    FLAGS+=" -DSAT_GUI_NOGUI"
    ;;
  "X11")
    #echo "gui = x11"
    FLAGS+=" -DSAT_GUI_X11"
    ;;
  *)
    echo "unknown gui: $GUI";
    exit 1
    ;;
esac

#------------------------------
# -d debug
#------------------------------

if $DEBUG
  then
    #echo "debug mode"
    FLAGS+=" -DSAT_DEBUG"
  else
    FLAGS+=" -DNDEBUG"
    FLAGS+=" -O3"
    FLAGS+=" -ffast-math"
    FLAGS+=" -s"
fi

#------------------------------
# -t test
#------------------------------

if $TEST
  then
    #echo "test mode"
    FLAGS+=" -DSAT_TEST"
#  else
fi

#----------------------------------------------------------------------
#
# compile
#
#----------------------------------------------------------------------

#echo "* format:" $FORMAT
#echo "* gui:" $GUI
#echo "* debug:" $DEBUG
#echo "* test:" $TEST

#echo
#echo "g++" $INPUT "-o" $OUTPUT $FLAGS $INC $LIB
#echo

g++ $INPUT -o $OUTPUT $FLAGS $INC $LIB

