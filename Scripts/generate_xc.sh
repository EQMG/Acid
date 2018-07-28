#!/usr/bin/env bash
cd ../

mkdir Build
cd Build
cmake .. -G "Xcode" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DACID_BUILD_TESTING=ON -DACID_INSTALL=OFF
xcodebuild -target Acid -configuration Release
