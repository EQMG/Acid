cd ../
mkdir Build
cd Build
cmake .. -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DACID_BUILD_TESTING=ON
cmake --build . --config Release --target install
pause
