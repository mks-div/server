@echo off
title start
cmake --build build || exit /b


cd build
server.exe