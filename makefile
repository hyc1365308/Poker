
ALL = Game.h Game.cpp Player.h

main : main.o Player.o
	g++ -o main main.o Player.o

main.o : Game.h Game.cpp Player.h
	g++ -c Game.cpp -o main.o -std=c++11

Player.o: Player.h Player.cpp
	g++ -c Player.cpp -o Player.o -std=c++11

clean:
	rm main.o main Player.o