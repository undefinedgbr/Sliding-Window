#include <netinet/in.h>
#include "../packet/frame.hpp"
#include "../sliding_window/sliding_window.hpp"
#include "../file/file_writer.hpp"

#ifndef SERVER_H
#define SERVER_H

namespace server
{
using namespace packet;
using namespace sw;
using namespace file;

class Server
{
  private:
	struct sockaddr_in address;
	int port;
	int sock;
	SlidingWindow window;
	char *destinationFile;

  public:
  	int bufferSize;
	FileWriter writer;
	Server(int port, char *destinationFile, int windowsize, int bufferSize);
	~Server();
	void listenForClients();
	void processFrame(Frame &frame, sockaddr clientAddress);
	void replyACK(int seqNum, sockaddr clientAddress);
	void replyNACK(int seqNum, sockaddr clientAddress);
	void checkAllFrames();
};
} // namespace server

#endif