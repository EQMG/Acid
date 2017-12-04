cd ../
mkdir Build-VS
cd Build-VS
cmake ../Source/ -G "Visual Studio 15 2017 Win64"
cmake --build . --config Release
pause
