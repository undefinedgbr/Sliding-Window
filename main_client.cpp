#include "client/client.hpp"
#include <thread>
#include <unistd.h>
#include <functional>
#include <string.h>
#include <vector>
#include <string>
#include "file/file_reader.hpp"

using namespace client;
using namespace std;
using namespace file;

int main(int argc, char **argv)
{
	if (argc != 6)
	{
		printf("Error invalid argument.\n");
		printf("Usage: %s <filename> <windowsize> <buffersize> <ip> <port>", argv[0]);
	}
	char *filename = argv[1];
	int windowsize = std::stoi(argv[2]);
	int buffersize = std::stoi(argv[3]);
	char *ip = argv[4];
	int port = std::stoi(argv[5]);

	printf("Starting %s at %s:%d window %d buffer %d sending %s\n", argv[0], ip, port, windowsize, buffersize, filename);

	char host[10];
	// string s = "127.0.0.1";
	string s = ip;
	strcpy(host, s.c_str());

	// Client client(host, 9000);
	Client client(host, port);

	thread thread1([&client] {
		client.listenForResponse();
	});

	thread thread2([&client] {
		client.resendIfTimeout();
	});

	// FileReader reader("examples/test.flac");
	FileReader reader(filename);
	vector<Frame> frames = reader.toFrames(1024);

	for (Frame f : frames)
	{
		f.generateChecksum();
		client.sendMessage(f);
		usleep(3000);
	}

	thread1.join();
	thread2.join();

	return 0;
}