CXX=g++
CXXFLAGS=-std=c++11
JSON = ./lib_json/json_reader.o ./lib_json/json_writer.o ./lib_json/json_value.o
GAME = ./game/Player.o ./game/Game.o ./game/Operate.o
HALL = ./hall.o
ROOM = ./room.o

$(GAME): ./game/Player.h ./game/Game.h ./game/Player.cpp ./game/Game.cpp

json: $(JSON)

game: $(GAME)

$(ROOM): room.h room.cpp
$(HALL): hall.h hall.cpp

room: $(ROOM)
	$(CXX) $(CXXFLAGS) -c room.cpp -o room.o

hall: $(HALL)
	$(CXX) $(CXXFLAGS) -c hall.cpp -o hall.o

server: game json room hall
	$(CXX) $(JSON) $(GAME) room.o hall.o server.cpp -lws2_32 -o server

clean:
	cd game && del Player.o
	cd game && del Game.o
