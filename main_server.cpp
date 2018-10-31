#include "server/server.hpp"
#include <stdio.h>
#include <string>

using namespace server;

int main(int argc, char **argv)
{
	if (argc != 5)
	{
		printf("Error invalid argument.\n");
		printf("Usage: %s <filename> <windowsize> <buffersize> <port>", argv[0]);
	}
	char *filename = argv[1];
	int windowsize = std::stoi(argv[2]);
	int buffersize = std::stoi(argv[3]);
	int port = std::stoi(argv[4]);

	printf("Starting %s at 0.0.0.0:%d window %d buffer %d receiving %s\n", argv[0], port, windowsize, buffersize, filename);

	Server server(port, filename);
	server.listenForClients();
	return 0;
}