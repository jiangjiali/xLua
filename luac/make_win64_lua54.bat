mkdir build64_54 & pushd build64_54
cmake -DLUA_VERSION=5.4.4 -DLUAC_COMPATIBLE_FORMAT=ON -G "Visual Studio 16 2019" -A x64 ..
popd
cmake --build build64_54 --config Release
md plugin_lua54\Plugins\luac
copy /Y build64_54\Release\luac.exe plugin_lua54\Plugins\luac\luac.exe
copy /Y build64_54\Release\lua.exe plugin_lua54\Plugins\luac\lua.exe

pause