cd ../
mkdir Build
cd Build
cmake .. -G "Visual Studio 15 2017 Win64"
cmake --build . --config Release
pause
