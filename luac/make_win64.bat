mkdir build64 & pushd build64
cmake -DLUAC_COMPATIBLE_FORMAT=ON -G "Visual Studio 16 2019" -A x64 ..
popd
cmake --build build64 --config Release
md plugin_lua54\Plugins\x86_64
copy /Y build64\Release\luac.exe plugin_lua54\Plugins\luac.exe

pause