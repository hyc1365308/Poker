CXX=g++
CXXFLAGS=-std=c++11
JSON = ./lib_json/json_reader.o ./lib_json/json_writer.o ./lib_json/json_value.o
GAME = ./game/Player.o ./game/Game.o


json: makefile 
	$(CXX) $(CXXFLAGS) -c ./lib_json/json_reader.cpp -o ./lib_json/json_reader.o
	$(CXX) $(CXXFLAGS) -c ./lib_json/json_writer.cpp -o ./lib_json/json_writer.o
	$(CXX) $(CXXFLAGS) -c ./lib_json/json_value.cpp  -o ./lib_json/json_value.o

game: ./game/Player.cpp ./game/Game.cpp
	$(CXX) $(CXXFLAGS) -c ./game/Player.cpp -o ./game/Player.o
	$(CXX) $(CXXFLAGS) -c ./game/Player.o ./game/Game.cpp -o ./game/Game.o

room: game
	$(CXX) $(CXXFLAGS) -c ./game/Game.o ./game/Player.o room.cpp -o room.o

server: json room
	$(CXX) $(JSON) $(GAME) room.o server.cpp -lws2_32 -o server && server

client: json
	$(CXX) $(CXXFLAGS) $(JSON) client.cpp -lws2_32  -o client

data: 
	$(CXX) $(JSON) data.cpp -o data
