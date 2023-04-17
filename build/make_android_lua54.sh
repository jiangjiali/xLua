if [ -n "$ANDROID_NDK" ]; then
    export NDK=${ANDROID_NDK}
elif [ -n "$ANDROID_NDK_HOME" ]; then
    export NDK=${ANDROID_NDK_HOME}
else
    export NDK=~/android-ndk-r21b
fi

if [ ! -d "$NDK" ]; then
    echo "Please set ANDROID_NDK environment to the root of NDK."
    exit 1
fi

function build() {
    API=$1
    ABI=$2
    TOOLCHAIN_ANME=$3
    BUILD_PATH=build54.Android.${ABI}
    # sudo apt install libbson-dev libmongoc-dev
    wget https://github.com/mongodb/mongo-c-driver/releases/download/1.23.3/mongo-c-driver-1.23.3.tar.gz
    tar xzf mongo-c-driver-1.23.3.tar.gz
    cd mongo-c-driver-1.23.3
    mkdir cmake-build
    cd cmake-build
    cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
    cd ../..
    # sed -i '455d' ${NDK}/build/cmake/android.toolchain.cmake
    sed '455s/.*/  -s/' ${NDK}/build/cmake/android.toolchain.cmake
    cmake -H. -B${BUILD_PATH} -DLUA_VERSION=5.4.3 -DANDROID_ABI=${ABI} -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=${NDK}/build/cmake/android.toolchain.cmake -DANDROID_NATIVE_API_LEVEL=${API} -DANDROID_TOOLCHAIN=clang -DANDROID_TOOLCHAIN_NAME=${TOOLCHAIN_ANME}
    cmake --build ${BUILD_PATH} --config Release
    mkdir -p plugin_lua54/Plugins/Android/libs/${ABI}/
    cp ${BUILD_PATH}/libxlua.so plugin_lua54/Plugins/Android/libs/${ABI}/libxlua.so
}

build android-26 armeabi-v7a arm-linux-androideabi-4.9
build android-26 arm64-v8a arm-linux-androideabi-clang
build android-26 x86 x86-4.9
