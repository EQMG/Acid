cd ../
mkdir Build
cd Build
cmake .. -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DFLOUNDER_BUILD_TESTING=OFF
cmake --build . --target install
pause
