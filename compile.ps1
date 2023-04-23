rm -R ./main.exe;
g++ main.cpp -I./sdl2/include -L./sdl2/lib -Wall -lmingw32 -lSDL2main -lSDL2_image -lSDL2 -o main;
./main.exe