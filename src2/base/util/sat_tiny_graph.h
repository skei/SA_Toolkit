#ifndef sat_tiny_graph_included
#define sat_tiny_graph_included
//----------------------------------------------------------------------

#include "sat.h"

//#include "s4k_data.h"
//#include "s4k_expr.h"

//----------------------------------------------------------------------

#define sat_tiny_graph_nodetype_empty      0
#define sat_tiny_graph_nodetype_noop       1
#define sat_tiny_graph_nodetype_proc       2
#define sat_tiny_graph_nodetype_expr       3
#define sat_tiny_graph_nodetype_cond       4
#define sat_tiny_graph_nodetype_repeat     5
#define sat_tiny_graph_nodetype_loc        6
#define sat_tiny_graph_nodetype_call       7
#define sat_tiny_graph_nodetype_cycle      8
#define sat_tiny_graph_nodetype_pattern    9
#define sat_tiny_graph_nodetype_wait       10
#define sat_tiny_graph_nodetype_poly       11
#define sat_tiny_graph_nodetype_voice      12

//----------------------------------------------------------------------

typedef void (*sat_tiny_graph_proc_t)(uint32_t node);

uint32_t sat_tiny_graph_graph_loc[256];   // setup in graph_init()
uint8_t  sat_tiny_graph_node_state[256];  // 0

//----------

uint8_t  sat_tiny_graph_node_type[256];
uint8_t  sat_tiny_graph_node_arg[256];
uint8_t  sat_tiny_graph_node_bro[256];  // Sibling
uint8_t  sat_tiny_graph_node_son[256];  // FirstChild

sat_tiny_graph_proc_t sat_tiny_graph_graph_proc[256];
uint8_t* sat_tiny_graph_graph_list[256]; // todo: 256*256 uint8
//uint8_t* sat_tiny_graph_graph_patt[256]; // todo: 256*256 uint8

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void sat_tiny_graph_graph_init(void);
void sat_tiny_graph_graph_parse(uint32_t self);
void sat_tiny_graph_graph_parse_child(uint32_t self, uint32_t child);
void sat_tiny_graph_graph_parse_children(uint32_t self);

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void sat_tiny_graph_graph_init(void) {
  uint32_t i;
  for (i=0; i<256; i++) {
    if (sat_tiny_graph_node_type[i] == sat_tiny_graph_nodetype_loc) {
      sat_tiny_graph_graph_loc[sat_tiny_graph_node_arg[i]] = i;
    }
    //sat_tiny_graph_node_state[i] = 0;
  }
}

//----------

void sat_tiny_graph_graph_parse_child(uint32_t self, uint32_t child) {
  uint32_t i;
  uint32_t node;
  i = 0;
  node = sat_tiny_graph_node_son[self];
  while (node) {
    if (i==child) sat_tiny_graph_graph_parse(node);
    i++;
    node = sat_tiny_graph_node_bro[node];
  }
}

//----------

void sat_tiny_graph_graph_parse_children(uint32_t self) {
  uint32_t node;
  node = sat_tiny_graph_node_son[self];
  while (node) {
    sat_tiny_graph_graph_parse(node);
    node = sat_tiny_graph_node_bro[node];
  }
}

//----------

void sat_tiny_graph_graph_parse(uint32_t self) {
  uint32_t op;
  uint32_t val;
  //printf("graph parse: %i\n",self);
  op = sat_tiny_graph_node_type[self];
  val = sat_tiny_graph_node_arg[self];
  switch (op) {
    //case sat_tiny_graph_node_empty:
    //  break;
    case sat_tiny_graph_nodetype_noop:
      //printf("yo!\n");
      sat_tiny_graph_graph_parse_children(self);
      break;
    case sat_tiny_graph_nodetype_proc:
      sat_tiny_graph_graph_proc[val](self);
      sat_tiny_graph_graph_parse_children(self);
      break;
    case sat_tiny_graph_nodetype_expr:
      //sat_tiny_graph_expr_evaluate( &sat_tiny_graph_list[val*256], false );
      //sat_tiny_graph_graph_parse_children(self);
      break;
    case sat_tiny_graph_nodetype_cond:
      //if (sat_tiny_graph_expr_evaluate( &sat_tiny_graph_list[val*256], true ) == true ) sat_tiny_graph_graph_parse_children(self);
      break;
    case sat_tiny_graph_nodetype_repeat:
      while (val-- > 0) sat_tiny_graph_graph_parse_children(self);
      break;
    case sat_tiny_graph_nodetype_loc:
      //graph_loc[val] = self;
      sat_tiny_graph_graph_parse_children(self);
      break;
    case sat_tiny_graph_nodetype_call:
      sat_tiny_graph_graph_parse(sat_tiny_graph_graph_loc[val]);
      sat_tiny_graph_graph_parse_children(self);
      break;
    case sat_tiny_graph_nodetype_wait:
      if (sat_tiny_graph_node_state[self] == 0) sat_tiny_graph_graph_parse_children(self);
      sat_tiny_graph_node_state[self]++;
      if (sat_tiny_graph_node_state[self] >= val) sat_tiny_graph_node_state[self] = 0;
      break;
    case sat_tiny_graph_nodetype_cycle:
      sat_tiny_graph_graph_parse_child(self, sat_tiny_graph_node_state[self]);
      sat_tiny_graph_node_state[self]++;
      if (sat_tiny_graph_node_state[self] >= val) sat_tiny_graph_node_state[self] = 0;
      break;
    case sat_tiny_graph_nodetype_pattern:
      //sat_tiny_graph_graph_parse_child(self, sat_tiny_graph_graph_list[sat_tiny_graph_node_state[self]]);
      //sat_tiny_graph_node_state[self]++;
      //if (sat_tiny_graph_node_state[self] >= val) sat_tiny_graph_node_state[self] = 0;
      break;
  } // switch op
}

//----------------------------------------------------------------------
#endif
