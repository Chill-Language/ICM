cd source
g++ -std=c++14 main.cpp ast.cpp tostring.cpp match.cpp parser.cpp objects.cpp keyword.cpp function.cpp deffunc.cpp typeobject.cpp order.cpp interpreter.cpp objectmanager.cpp ../include/common/number.cpp ../include/common/range.cpp -I../include -I../include/common -o ../main
../main
