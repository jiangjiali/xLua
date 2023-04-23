mkdir build64_54 & pushd build64_54
cmake -DLUA_VERSION=5.4.4 -DLUAC_COMPATIBLE_FORMAT=ON -G "Visual Studio 16 2019" -A x64  ..
popd
cmake --build build64_54 --config Release
md plugin_lua54\Plugins\x86_64
copy /Y build64_54\Release\xlua.dll plugin_lua54\Plugins\x86_64\xlua.dll

pause