#include "client/client.hpp"
#include <thread>
#include <unistd.h>
#include <functional>
#include <string.h>
#include <vector>
#include "file/file_reader.hpp"

using namespace client;
using namespace std;
using namespace file;

int main() {
	char host[10];
	string s = "127.0.0.1";
	strcpy(host, s.c_str());

	Client client(host, 9000);
	thread thread1([&client] {
		client.listenForResponse();
	});

	thread thread2([&client]{
		client.resendIfTimeout();
	});

	FileReader reader("examples/test.flac");
	vector<Frame> frames = reader.toFrames(1024);

	for (Frame f : frames) {
		f.generateChecksum();
		client.sendMessage(f);
		usleep(3000);
	}

	thread1.join();
	thread2.join();

	return 0;
}