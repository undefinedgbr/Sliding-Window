#include <netinet/in.h>
#include "../packet/frame.hpp"
#include "../sliding_window/sliding_window.hpp"

#ifndef SERVER_H
#define SERVER_H

namespace server {
	using namespace packet;
	using namespace sw;

	class Server {
	private:
		struct sockaddr_in address;
		int port;
		int sock;
		SlidingWindow window;
	public:
		Server(int port);
		~Server();
		void listenForClients();
		void processFrame(Frame& frame, sockaddr clientAddress);
		void replyACK(int seqNum, sockaddr clientAddress);
		void replyNACK(int seqNum, sockaddr clientAddress);
		void checkAllFrames();
	};
}

#endif