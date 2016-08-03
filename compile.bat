@echo off
cd source
g++ -std=c++11 main.cpp ast.cpp tostring.cpp match.cpp parser.cpp objects.cpp runast.cpp keyword.cpp function.cpp deffunc.cpp -I..\include -I..\include\common -o ..\main && ..\main
pause
