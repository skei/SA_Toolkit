#!/bin/bash

#----------------------------------------------------------------------
#
#
#
#----------------------------------------------------------------------

FLAGS=""
INC=""
LIB=""

FLAGS+=" -shared"
FLAGS+=" -fPIC"
FLAGS+=" -std=gnu++17"
#FLAGS+=" -Wall"
FLAGS+=" -Wno-unknown-pragmas -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -Wno-narrowing"
FLAGS+=" -pthread"
FLAGS+=" -O3"
FLAGS+=" -ffast-math"
FLAGS+=" -s"
FLAGS+=" -Wl,--as-needed"
FLAGS+=" -static-libstdc++"
FLAGS+=" -static-libgcc"

INC+=" -I../src"
INC+=" -I../src/extern/vst3/vst3sdk"

LIB+=" -ldl -lrt"
LIB+=" -lX11 -lX11-xcb -lxcb -lxcb-util -lxcb-image -lxcb-cursor -lxkbcommon -lxcb-keysyms"
LIB+=" -lGL -lGLU -lGLX"

#----------------------------------------------------------------------
#
# print help
#
#----------------------------------------------------------------------

function print_help {
  echo "usage: ./compile.sh <flags>"
  echo "  -i input file name/path"
  echo "  -o output file name/path"
  echo "  -f format (CLAP, vst2, vst3, ladspa, dssi, lv2, exe)"
  echo "  -g gui (NOGUI,xcb,win32)"
  echo "  -h this help"
  exit 1
}

#----------------------------------------------------------------------
#
# parse command line
#
#----------------------------------------------------------------------

if [ "$#" -eq 0 ]; then
  print_help
fi

#----------

while getopts i:o:f:g:h opt
do
  case "$opt" in
    i) INPUT="$OPTARG";;
    o) OUTPUT="$OPTARG";;
    f) FORMAT="$OPTARG";;
    g) GUI="$OPTARG";;
    h) print_help;;
  esac
done

#------------------------------
# input
#------------------------------

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
# output
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


#------------------------------
#
#------------------------------

#if [ "$OUTPUT" = "" ]; then
#  OUTPUT=$INPUT_BASE
#fi

# ----------

#if [ "$DEBUG" = "on" ]; then
#  FLAGS+="-DMIP_DEBUG "
#fi

#------------------------------
#
#------------------------------

g++ $INPUT -o ../bin/$OUTPUT $FLAGS $INC $LIB

