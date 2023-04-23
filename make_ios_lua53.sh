mkdir -p build_ios && cd build_ios
cmake -DPLATFORM=OS64 -DLUA_VERSION=5.3.5 -DLUAC_COMPATIBLE_FORMAT=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build_ios --config Release
mkdir -p plugin_lua53/Plugins/iOS/
cp build_ios/Release-iphoneos/libxlua.a plugin_lua53/Plugins/iOS/libxlua.a 
