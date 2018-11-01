#include "client.hpp"
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
#include <tuple>
#include "../packet/ack.hpp"

namespace client {
	using namespace packet;
	using namespace sw;
	using std::chrono::system_clock;
	using namespace std;

	Client::Client(char * host, int port) {
		this->port = port;
		this->serverAddress.sin_family = AF_INET; 
	    this->serverAddress.sin_addr.s_addr = inet_addr(host); 
	    this->serverAddress.sin_port = htons(port);
	    //inet_pton(AF_INET, host, &(this->serverAddress).sin_addr);
	    this->sock = socket(AF_INET, SOCK_DGRAM, 0);
	    this->window = SlidingWindow(10);
	}

	Client::~Client() {
		close(this->sock);
	}

	void Client::listenForResponse() {
		char * buffer;
		while (1) {
			buffer = new char[6];
			recv(this->sock, buffer, 6, 0);
			ACK ack(buffer);
			if (ack.checkValidity()) {
				printf("RECEIVED ACK : %d\n", ack.getNextSeqNum() -1);
				this->window.addACK(ack);
			}
			delete [] buffer;
		}
	}

	bool Client::sendMessage(Frame& frame) {
		char * buffer = frame.serialize();
		bool result = this->window.addFrame(frame);
		if (!result) {
			return false;
		}
		int res = sendto((this->sock) , buffer , 1034 , 0, (struct sockaddr *)&(this->serverAddress), sizeof(this->serverAddress)); 
		this->queueTimeout(frame.getSeqNum());
		printf("SENT FRAME : %d\n", frame.getSeqNum());
		free(buffer);
		return true;
	}

	bool Client::checkACK(int seqNum) {
		ACK& ack = this->window.getACK(seqNum);
		return ack.getAck() == 0x6 && ack.getNextSeqNum() == (seqNum + 1);
	}

	void Client::resendMessage(int seqNum) {
		if (this->checkACK(seqNum)) {
			return;
		}

		Frame& frame = this->window.getFrame(seqNum);
		frame.generateChecksum();
		this->sendMessage(frame);
	}

	void Client::queueTimeout(int seqNum) {
		std::time_t tt = system_clock::to_time_t (system_clock::now());
		struct std::tm * ptm = std::localtime(&tt);
		ptm->tm_sec += 1;
		this->timeouts.push_back(std::make_tuple(seqNum, ptm));
	}

	void Client::resendIfTimeout() {
		while (1) {
			if (this->timeouts.empty()) {
				printf("EMPTY\n");
				usleep(100);
				continue;
			}
			printf("TIMEOUT\n");
			tuple<int, struct std::tm *> timeout = this->timeouts.front();
			this_thread::sleep_until (system_clock::from_time_t (mktime(std::get<1>(timeout))));
			this->timeouts.pop_front();
			if (!this->checkACK(std::get<0>(timeout))) {
				printf("RESEND : %d\n", std::get<0>(timeout));
				resendMessage(std::get<0>(timeout));
				this->timeouts.push_back(timeout);
			}
		}
	}

}