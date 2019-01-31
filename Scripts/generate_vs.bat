cd ../
mkdir Build
cd Build
cmake -G"Visual Studio 15 2017 Win64" ..
pause
cmake --build . --config Release --target install --clean-first
pause
