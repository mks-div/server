@echo off
if exist build (
    rmdir /s /q build
    mkdir build
)
exit