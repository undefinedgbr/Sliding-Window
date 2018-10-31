CXX = g++
CXXFLAGS = -std=c++14 -pthread

CLIENT = client.out
SERVER = server.out
DEPENDENCY = packet/ack.cpp packet/frame.cpp packet/packet.cpp sliding_window/sliding_window.cpp


all: $(CLIENT) $(SERVER)

$(CLIENT) : main_client.cpp client/client.cpp packet/ack.cpp packet/frame.cpp packet/packet.cpp
	$(CXX) $(CXXFLAGS) main_client.cpp client/client.cpp $(DEPENDENCY) -o $(CLIENT)

$(SERVER): main_server.cpp server/server.cpp packet/ack.cpp packet/frame.cpp packet/packet.cpp
	$(CXX) $(CXXFLAGS) main_server.cpp server/server.cpp $(DEPENDENCY) -o $(SERVER)