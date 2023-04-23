rm -R ./main.exe;
g++ main.cpp -I./include -L./lib -Wall -lmingw32 -lSDL2main -lSDL2_image -lSDL2 -o main;
./main.exe