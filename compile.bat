@echo off
cd source
g++ -std=c++11 main.cpp ast.cpp objectdata.cpp tostring.cpp match.cpp parser.cpp objects.cpp -I..\include -I..\include\common -o main && main
pause
