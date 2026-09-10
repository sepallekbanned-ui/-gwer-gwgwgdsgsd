@echo off
setlocal
where cmake >nul 2>nul
if errorlevel 1 (
  echo CMake was not found. Install CMake 3.25+ and Visual Studio 2022 Desktop C++.
  pause
  exit /b 1
)
if not exist build mkdir build
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
if errorlevel 1 goto :fail
cmake --build build --config Release
if errorlevel 1 goto :fail
echo.
echo BUILD OK:
echo build\bin\moza_ats_blinker.dll
echo build\bin\MOZA_SDK.dll
pause
exit /b 0
:fail
echo BUILD FAILED.
pause
exit /b 1
