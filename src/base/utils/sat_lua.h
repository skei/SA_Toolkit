#ifndef sat_lua_included
#define sat_lua_included
//----------------------------------------------------------------------

#include <lua5.3/lua.hpp>
// -llua5.3

//----------------------------------------------------------------------

void test_lua() {
  lua_State* L = luaL_newstate();
  luaL_dostring(L, "a = 10 + 5");
  lua_getglobal(L, "a");
  int i = lua_tointeger(L, -1);
  SAT_Print("%d\n", i);
  lua_close(L);
}

//----------------------------------------------------------------------
#endif
