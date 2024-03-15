#!/usr/bin/tcc -run

// in progress..
// from: https://stackoverflow.com/questions/9642732/parsing-command-line-arguments-in-c
// adapting to replace the compile script (in /build)

#include <argp.h>
#include <stdbool.h>

//----------------------------------------------------------------------

struct arguments {
  enum {
    CHARACTER_MODE,
    WORD_MODE,
    LINE_MODE
  } mode;
  bool isCaseInsensitive;
};

//----------------------------------------------------------------------

static struct argp_option options[] = { 
  { "line",   'l', 0, 0, "Compare lines instead of characters."},
  { "word",   'w', 0, 0, "Compare words instead of characters."},
  { "nocase", 'i', 0, 0, "Compare case insensitive instead of case sensitive."},
  { 0 } 
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
    case 'l':           arguments->mode = LINE_MODE;          break;
    case 'w':           arguments->mode = WORD_MODE;          break;
    case 'i':           arguments->isCaseInsensitive = true;  break;
    case ARGP_KEY_ARG:  return 0;
    default:            return ARGP_ERR_UNKNOWN;
  }   
  return 0;
}

//static char doc[] = "Your program description.";
//static char args_doc[] = "[FILENAME]...";

static struct argp argp = {
  options,
  parse_opt,
  "[FILENAME]...",              // args_doc,
  "Your program description.",  // doc,
  0,                            // children
  0,                            // help_filter
  0                             // argp_domain
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main(int argc, char *argv[]) {

  struct arguments arguments;
  arguments.mode = CHARACTER_MODE;
  arguments.isCaseInsensitive = false;

  argp_parse(
    &argp,
    argc,
    argv,
    0,          // flags
    0,          // arg_index
    &arguments  // input
  );

  //----------

}

