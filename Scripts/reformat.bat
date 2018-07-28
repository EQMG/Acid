cd ../Sources/

For /R %%G in (*.hpp, *.cpp) do clang-format.exe -style=file -i -fallback-style=none -sort-includes "%%G"
pause