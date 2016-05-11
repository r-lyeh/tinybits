#/bin/bash 2>nul || goto :windows

# bash
echo hello Bash
ls
exit

:windows
@echo off
echo hello Windows
ver
exit /b
