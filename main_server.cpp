#include "server/server.hpp"

using namespace server;

int main() {
	Server myServer(9000);
	myServer.listenForClients();
	return 0;
}