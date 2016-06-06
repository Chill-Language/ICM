@echo off
cd source
g++ -std=c++11 main.cpp ast.cpp objectdata.cpp tostring.cpp -I%~dp0\include -o main && main
pause
