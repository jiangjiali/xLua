mkdir -p build_osx && cd build_osx
cmake -DLUA_VERSION=5.3.5 -DLUAC_COMPATIBLE_FORMAT=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build_osx --config Release
mkdir -p plugin_lua53/Plugins/xlua.bundle/Contents/MacOS/
cp build_osx/Release/xlua.bundle/Contents/MacOS/xlua plugin_lua53/Plugins/xlua.bundle/Contents/MacOS/xlua
