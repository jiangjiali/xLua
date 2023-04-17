## C#下Lua编程支持

xLua为Unity、 .Net、 Mono等C#环境增加Lua脚本编程的能力，借助xLua，这些Lua代码可以方便的和C#相互调用。

## 执行文件的权限问题
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