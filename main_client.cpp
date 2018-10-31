#include "client/client.hpp"
#include <thread>
#include <unistd.h>
#include <functional>
#include <string.h>

using namespace client;
using namespace std;

int main() {
	char host[10];
	std::string s = "127.0.0.1";
	strcpy(host, s.c_str());

	Client client(host, 9000);
	thread thread1([&client] {
		client.listenForResponse();
	});

	thread thread2([&client]{
		client.resendIfTimeout();
	});

	char data[1024];

	for (int i = 0; i < 1024; i++) {
		data[i] = 'x';
	}

	Frame frame(0x1, 0, 1024, data);

	for (int i = 0; i < 10000; i++) {
		frame.setSeqNum(i);
		if (i % 2) {
			frame.generateChecksum();
		}
		client.sendMessage(frame);
		usleep(500);
	}

	thread1.join();
	thread2.join();

	return 0;
}