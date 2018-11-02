#include "ack.hpp"
#include <stdlib.h>

namespace packet {
	ACK::ACK() {
		this->ack = 0;
		this->nextSeqNum = -1;
		this->checksum = -1;
	}

	ACK::ACK(const ACK& ack) {
		this->ack = ack.ack;
		this->nextSeqNum = ack.nextSeqNum;
		this->checksum = ack.checksum;
	}

	ACK::ACK(char ack, int nextSeqNum) {
		this->ack = ack;
		this->nextSeqNum = nextSeqNum;
		this->generateChecksum();
	}

	ACK::ACK(char * serial) {
		this->ack = serial[0];
		this->nextSeqNum = int(((unsigned char)serial[1] << 24) & 0xFF000000 |
						   ((unsigned char)serial[2] << 16) & 0x00FF0000 |
						   ((unsigned char)serial[3] << 8) & 0x0000FF00 |
						   ((unsigned char)serial[4]) & 0x000000FF);
		this->checksum = serial[5];
	}

	char * ACK::serialize() const {
		char * serial;
		serial = (char *) malloc(6);

		serial[0] = this->ack;
		for (int i = 1; i < 5; i++) {
			serial[i] = this->nextSeqNum >> ((4-i) * 8);
		}
		serial[5] = this->checksum;
		return serial;
	}

	char ACK::getAck() {
		return this->ack;
	}

	int ACK::getNextSeqNum() {
		return this->nextSeqNum;
	}

	void ACK::setAck(char ack) {
		this->ack = ack;
	}

	void ACK::setNextSeqNum(int nextSeqNum) {
		this->nextSeqNum = nextSeqNum;
	}

	void ACK::generateChecksum() {
		unsigned short shortSum = 0;
		unsigned char sum = 0;
		unsigned char temp = 0;
		unsigned char sumTemp = 0;
		char * serial = this->serialize();

		for (int i = 0; i < 5; i += 1) {
			temp = (unsigned char) serial[i];
			shortSum = sum + temp;
			sum = sum + temp;
			if (shortSum >= sum) {
				sum += 1;
			}
		}

		this->checksum = ~sum;
	}
}