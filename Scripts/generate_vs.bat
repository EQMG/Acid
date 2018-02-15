cd ../
mkdir Build
cd Build
cmake .. -G "Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=Debug
cmake --build .
pause
