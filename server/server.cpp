#include <stdio.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

#include "server.hpp"
#include "../packet/ack.hpp"
#include "../packet/frame.hpp"
#include "../file/file_writer.hpp"

namespace server
{
using namespace packet;
using namespace sw;
using namespace file;

Server::Server(int port, char *destinationFile) : writer(destinationFile)
{
	this->destinationFile = destinationFile;
	this->port = port;
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(port);
	this->window = SlidingWindow(10);
	this->sock = socket(AF_INET, SOCK_DGRAM, 0);
	bind((this->sock), (struct sockaddr *)&(this->address), sizeof(this->address));

	this->window.setWFCallback([this](vector<Frame> &frames) {
		printf("DUMPED CALLED\n");
		this->window.locked = true;
		this->writer.appendPacket(frames);
		this->window.frames.clear();
		this->window.frames.resize(100);
		this->window.locked = false;
	});
}

Server::~Server()
{
	close(this->sock);
}

void Server::listenForClients()
{
	sockaddr clientAddress;
	unsigned int addrlen;
	char *buffer;
	int dataLength;
	bool lastPacketReceived = false;
	while (1)
	{
		buffer = new char[1034];
		printf("WAIT\n");
		recvfrom((this->sock), buffer, 1034, 0, &(clientAddress), &addrlen);
		Frame frame(buffer);
		if (!lastPacketReceived)
		{
			lastPacketReceived = frame.getSOH() == 0x4;
		}
		printf("RECEIVED FRAME : %d\n", frame.getSeqNum());
		this->processFrame(frame, clientAddress);
		if (lastPacketReceived)
		{
			//writeFramesToFile("examples/test-result.pdf", this->window.frames);
			this->checkAllFrames();
		}
		delete[] buffer;
	}
}

void Server::processFrame(Frame &frame, sockaddr clientAddress)
{
	if (frame.checkValidity())
	{
		this->window.addFrame(frame);
		this->replyACK(frame.getSeqNum(), clientAddress);
	}
	else
	{
		this->replyNACK(frame.getSeqNum(), clientAddress);
	}
}

void Server::replyACK(int seqNum, sockaddr clientAddress)
{
	ACK ack(0x6, seqNum + 1);
	printf("SENT ACK : %d\n", ack.getNextSeqNum() - 1);
	this->window.addACK(ack);
	char *buffer = ack.serialize();
	sendto(this->sock, ack.serialize(), 6, 0, &(clientAddress), sizeof(clientAddress));
	free(buffer);
}

void Server::replyNACK(int seqNum, sockaddr clientAddress)
{
	ACK ack(0x2, seqNum + 1);
	printf("SENT NACK : %d\n", ack.getNextSeqNum() - 1);
	this->window.addACK(ack);
	char *buffer = ack.serialize();
	sendto(this->sock, ack.serialize(), 6, 0, &(clientAddress), sizeof(clientAddress));
	free(buffer);
}

void Server::checkAllFrames()
{
	this->window.locked = true;
	printf("FRAME LEFT %d\n", this->window.frames.size());
	int i = 0;
	for (Frame &f : this->window.frames)
	{
		if (f.getSeqNum() != i + this->window.dumped)
		{
			printf("MISSING FRAME : %d\n", i);
			break;
		}
		i++;
	}

	if (this->window.getFrame(i - 1 + this->window.dumped).getSOH() == 0x4)
	{
		printf("CLEAARRR\n");
		this->writer.appendPacket(this->window.frames);
		printf("EXIT\n");
	}
	this->window.locked = false;
	// exit(0);
}
} // namespace server
