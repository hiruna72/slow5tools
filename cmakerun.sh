#!/bin/sh

# for armeabi-v7a and arm64-v8a cross compiling
toolchain_file="update this with path to 'android-sdk-linux/ndk-bundle/build/cmake/android.toolchain.cmake'"

# terminate script
die(){
	echo "$1"
	exit 1
}


mkdir -p build_ninja
rm -rf build_ninja
mkdir build_ninja
cd build_ninja

# for architecture x86
 # cmake .. -DDEPLOY_PLATFORM=x86
 # make -j 8

# # for architecture armeabi-V7a
cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE:STRING=$toolchain_file -DANDROID_PLATFORM=android-21 -DDEPLOY_PLATFORM:STRING="armeabi-v7a" -DANDROID_ABI="armeabi-v7a"
ninja

# # for architecture arm66-v8a
# cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE:STRING=$toolchain_file -DANDROID_PLATFORM=android-23 -DDEPLOY_PLATFORM:STRING="arm64-v8a" -DANDROID_ABI="arm64-v8a"
# ninja
