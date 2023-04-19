#include <lua.hpp>
#include <f_math.h>

// 函数返回两个数中较大的那个数
static int add(lua_State *L) {
    int num1 = lua_tonumber(L, 1);
    int num2 = lua_tonumber(L, 2);
    int sum = math::add(num1,num2);
    lua_pushnumber(L, sum);
    return 1;
}

//注册函数
static const luaL_Reg methods[] = {
        {"add",   add},
        {nullptr, nullptr}
};

extern "C" {

LUALIB_API int lua_framework(lua_State *L) {
    luaL_newlib(L, methods);

    lua_pushliteral(L, "framework");
    lua_setfield(L, -2, "_NAME");

    return 1;
}
}