cd ../
mkdir Build
cd Build
cmake .. -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=OFF -DACID_BUILD_TESTING=ON -DACID_INSTALL=ON
mkdir Tests\TestGuis\Resources
mkdir Tests\TestPhysics\Resources
pause
cmake --build . --config Debug --target install
pause
