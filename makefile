CXX=g++
CXXFLAGS=-std=c++11
JSON = ./lib_json/json_reader.o ./lib_json/json_writer.o ./lib_json/json_value.o
GAME = ./game/Player.o ./game/Game.o

$(GAME): ./game/Player.h ./game/Game.h ./game/Player.cpp ./game/Game.cpp

json: $(JSON)

game: $(GAME)
	$(CXX) $(CXXFLAGS) -c ./game/Player.cpp -o ./game/Player.o
	$(CXX) $(CXXFLAGS) -c ./game/Game.cpp   -o ./game/Game.o

room: room.h room.cpp
	$(CXX) $(CXXFLAGS) -c room.cpp -o room.o

hall: hall.h hall.cpp
	$(CXX) $(CXXFLAGS) -c hall.cpp -o hall.o

server: game json room hall
	$(CXX) $(JSON) $(GAME) room.o hall.o server.cpp -lws2_32 -o server

server2: json game room
	$(CXX) $(JSON) $(GAME) room.o hall.o server.cpp -lws2_32 -o server2

client: json
	$(CXX) $(CXXFLAGS) ./lib_json/*.o client.cpp -lws2_32 -o client

data: 
	$(CXX) $(JSON) data.cpp -o data

clean:
	cd game && del Player.o
	cd game && del Game.o
