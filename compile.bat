@echo off
cd source
set src= main.cpp ast.cpp tostring.cpp match.cpp parser.cpp objects.cpp runast.cpp keyword.cpp function.cpp deffunc.cpp typeobject.cpp
set incdir= ..\include
set comdir= %incdir%\common
set comsrc= %comdir%\number.cpp %comdir%\range.cpp

g++ -s -Wall -std=c++14 %src% %comsrc% -o ..\main -I%comdir% -I%incdir%

cd ..

pause