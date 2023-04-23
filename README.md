## C#下Lua编程支持
xLua为Unity、 .Net、 Mono等C#环境增加Lua脚本编程的能力，借助xLua，这些Lua代码可以方便的和C#相互调用。

### 编译库
* 移除 yasio v3.39.9 https://github.com/yasio/yasio
* 移除 lua-sqlite3 v0.9.5 http://lua.sqlite.org/index.cgi/home
* lua-rapidjson v0.7.1 https://github.com/xpol/lua-rapidjson
* 移除 lua-protobuf v0.4.0 https://github.com/starwing/lua-protobuf

### 依赖库
* yasio 依赖 c-ares v1.19.0 https://github.com/c-ares/c-ares
* yasio 依赖 sol v3.3.0 https://github.com/ThePhD/sol2
* yasio 依赖 sol2 v3.3.0 https://github.com/ThePhD/sol2

## 执行文件的权限问题
* git update-index --chmod=+x make_android_lua53.sh
* git update-index --chmod=+x make_android_lua54.sh

## Lua版本修改
```
# 文件名：loslib.c 修改文件。移除 os_execute 函数

// static int os_execute (lua_State *L) {
//   const char *cmd = luaL_optstring(L, 1, NULL);
//   int stat;
//   errno = 0;
//   stat = system(cmd);
//   if (cmd != NULL)
//     return luaL_execresult(L, stat);
//   else {
//     lua_pushboolean(L, stat);  /* true if there is a shell */
//     return 1;
//   }
// }


static const luaL_Reg syslib[] = {
  {"clock",     os_clock},
  {"date",      os_date},
  {"difftime",  os_difftime},
  // {"execute",   os_execute},
  {"exit",      os_exit},
  {"getenv",    os_getenv},
  {"remove",    os_remove},
  {"rename",    os_rename},
  {"setlocale", os_setlocale},
  {"time",      os_time},
  {"tmpname",   os_tmpname},
  {NULL, NULL}
};
```


```
# 文件名：luaconf.h 把文件改名为luaconf.h.in 并修改如下

#ifdef __ANDROID__
#define lua_getlocaledecpoint() '.'
#elif !defined(lua_getlocaledecpoint)
#define lua_getlocaledecpoint()		(localeconv()->decimal_point[0])
#endif
```