mkdir build64 & pushd build64
cmake -DLUA_VERSION=5.3.5 -DLUAC_COMPATIBLE_FORMAT=ON -G "Visual Studio 16 2019" -A x64  ..
popd
cmake --build build64 --config Release
md plugin_lua53\Plugins\x86_64
copy /Y build64\Release\xlua.dll plugin_lua53\Plugins\x86_64\xlua.dll

pause