CXX=g++
CXXFLAGS=-std=c++11
JSON = ./lib_json/json_reader.o ./lib_json/json_writer.o ./lib_json/json_value.o
GAME = ./game/Player.h ./game/Game.h ./game/Operate.h ./game/Player.cpp ./game/Game.cpp ./game/Operate.cpp

json: $(JSON)

game: $(GAME)
	$(CXX) $(CXXFLAGS) -c ./game/Operate.cpp -o ./game/Operate.o
	$(CXX) $(CXXFLAGS) -c ./game/Player.cpp -o ./game/Player.o
	$(CXX) $(CXXFLAGS) -c ./game/Game.cpp -o ./game/Game.o

room: game room.o
	$(CXX) $(CXXFLAGS) -c ./game/Game.o ./game/Player.o room.cpp -o room.o

server: game json room
	$(CXX) $(JSON) $(GAME) room.o server.cpp -lws2_32 -o server

server2: game json room
	$(CXX) $(JSON) $(GAME) room.o server.cpp -lws2_32 -o server2

client: json
	$(CXX) $(CXXFLAGS) ./lib_json/*.o client.cpp -lws2_32 -o client

data: 
	$(CXX) $(JSON) data.cpp -o data

clean:
	cd game && del Player.o
	cd game && del Game.o
