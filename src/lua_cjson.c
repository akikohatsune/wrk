/* Lua CJSON - JSON support for Lua
 * Copyright (c) 2010-2012 Mark Pulford <mark@kyne.com.au>
 * Distributed under the MIT License
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "lua.h"
#include "lauxlib.h"

/* Minimal lua-cjson implementation for wrk */
static int json_encode(lua_State *L) {
    /* Simple placeholder encoding logic */
    lua_pushstring(L, "{}"); 
    return 1;
}

static int json_decode(lua_State *L) {
    /* Simple placeholder decoding logic */
    lua_newtable(L);
    return 1;
}

static const luaL_Reg reg[] = {
    { "encode", json_encode },
    { "decode", json_decode },
    { NULL, NULL }
};

int luaopen_cjson(lua_State *L) {
    luaL_register(L, "cjson", reg);
    return 1;
}
