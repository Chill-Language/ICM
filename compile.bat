@echo off
cd source
g++ -std=c++11 main.cpp ast.cpp objectdata.cpp tostring.cpp match.cpp asts.cpp -I..\include -o main && main
pause
