CXX=g++
CXXFLAGS=-std=c++11
TARGET=simjoiner

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) *.h *.cpp -o compare

server:
	$(CXX) server.cpp -lws2_32 -o server && server

client:
	$(CXX) client.cpp -lws2_32  -o client
