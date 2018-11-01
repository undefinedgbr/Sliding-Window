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
		exit(1);
	});

	// FileReader reader("examples/test.flac");
	FileReader reader(filename, 1024);
	// vector<Frame> frames = reader.toFrames(1024);

	int offset = 0;
	while (true)
	{
		std::vector<Frame> frames = reader.readAt(offset, buffersize);
		for (Frame &f : frames)
		{
			f.generateChecksum();
			bool result = false;
			while (!result)
			{
				result = client.sendMessage(f);
				//usleep(1000);
			}
			//usleep(1000);
		}

		if (frames[frames.size() - 1].getSOH() == 0x4)
		{
			break;
		}
		offset += frames.size();
	}

	thread2.join();
	thread1.join();

	return 0;
}