@echo off
title start
mkdir build

cmake -DCMAKE_BUILD_TYPE=Debug ^
      -DCMAKE_C_COMPILER=C:\msys64\ucrt64\bin\gcc.exe ^
      -DCMAKE_CXX_COMPILER=C:\msys64\ucrt64\bin\g++.exe ^
      -DCMAKE_CXX_STANDARD=20 ^
      -S . ^
      -B build ^
      -G "MinGW Makefiles" || exit /b
pause
