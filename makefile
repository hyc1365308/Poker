CXX=g++
CXXFLAGS=-std=c++11
TARGET=simjoiner

json: ./lib_json/json_reader.o ./lib_json/json_writer.o ./lib_json/json_value.o

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) *.h *.cpp -o compare

server: json room.o
	$(CXX) ./lib_json/*.o room.o server.cpp -lws2_32 -o server && server

client: json
	$(CXX) ./lib_json/*.o client.cpp -lws2_32  -o client

data: 
	$(CXX) ./lib_json/*.o data.cpp -o data
