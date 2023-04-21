mkdir build64 & pushd build64
cmake -DLUAC_COMPATIBLE_FORMAT=ON -G "Visual Studio 14 2015 Win64" ..
popd
cmake --build build64 --config Release
md plugin_lua53\Plugins\luac
copy /Y build64\Release\luac.exe plugin_lua53\Plugins\luac\luac.exe
copy /Y build64\Release\lua.exe plugin_lua53\Plugins\luac\lua.exe

pause

