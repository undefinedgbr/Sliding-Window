#include <netinet/in.h>
#include <string>
#include <deque>
#include <tuple>
#include "../packet/frame.hpp"
#include "../sliding_window/sliding_window.hpp"
#include <ctime>

#ifndef CLIENT_H
#define CLIENT_H

namespace client
{
using namespace packet;
using namespace sw;
using namespace std;

class Client
{
  private:
	struct sockaddr_in serverAddress;
	int port;
	int sock;
	SlidingWindow window;
	deque<tuple<int, struct tm *>> timeouts;

  public:
	Client(char *host, int port);
	~Client();
	void sendMessage(Frame &frame);
	bool checkACK(int seqNum);
	void resendMessage(int seqNum);
	void listenForResponse();
	void queueTimeout(int seqNum);
	void resendIfTimeout();
};
} // namespace client

#endif