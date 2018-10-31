#include "server/server.hpp"

using namespace server;

int main() {
	Server server(9000);
	server.listenForClients();
	return 0;
}