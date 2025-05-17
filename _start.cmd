@echo off
title start
cmake --build -DCMAKE_CXX_STANDARD=20 build || exit /b


cd build
server.exe