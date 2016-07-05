cd source
g++ -std=c++11 main.cpp ast.cpp tostring.cpp match.cpp parser.cpp objects.cpp runast.cpp -I../include -I../include/common -o ../main
../main
