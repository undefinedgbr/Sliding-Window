#include "frame.hpp"
#include <stdlib.h>
#include <stdio.h>


namespace packet {
	int Frame::frameCount(0);
	Frame::Frame() {
		frameCount++;
		this->SOH = 0;
		this->seqNum = 0;
		this->dataLength = 0;
		this->checksum = -1;
	}

	Frame::Frame(char SOH, int seqNum, int dataLength, char * data) {
		frameCount++;
		this->SOH = SOH;
		this->seqNum = seqNum;
		this->dataLength = dataLength;

		for (int i = 0; i < this->dataLength; i++) {
			this->data[i] = data[i];
		}

		this->generateChecksum();
	}

	Frame::Frame(char *serial) {
		printf("ALIVE FRAMES : %d\n", frameCount);
		frameCount++;
		this->SOH = serial[0];
		this->seqNum = int(((unsigned char)serial[1] << 24) & 0xFF000000 |
						   ((unsigned char)serial[2] << 16) & 0x00FF0000 |
						   ((unsigned char)serial[3] << 8) & 0x0000FF00 |
						   ((unsigned char)serial[4]) & 0x000000FF);

		this->dataLength = int(((unsigned char)serial[5] << 24) & 0xFF000000 |
							   ((unsigned char)serial[6] << 16) & 0x00FF0000 |
							   ((unsigned char)serial[7] << 8) & 0x0000FF00 |
							   ((unsigned char)serial[8]) & 0x000000FF);

		for (int i = 0; i < this->dataLength; i++) {
			this->data[i] = serial[i + 9];
		}

		this->checksum = (unsigned char)serial[this->dataLength + 9];
	}

	Frame::Frame(const Frame &frame) : Frame(frame.serialize()) {}

	Frame::~Frame() {
		frameCount--;
	}

	char *Frame::serialize() const {
		char *serial;
		serial = (char *)malloc(this->dataLength + 10);

		serial[0] = this->SOH;
		serial[this->dataLength + 9] = this->checksum;

		for (int i = 1; i < 5; i++)
		{
			serial[i] = this->seqNum >> ((4 - i) * 8) & 0xFF;
		}

		serial[5] = (this->dataLength >> 24) & 0xFF;
		serial[6] = (this->dataLength >> 16) & 0xFF;
		serial[7] = (this->dataLength >> 8) & 0xFF;
		serial[8] = this->dataLength & 0xFF;

		for (int i = 0; i < this->dataLength; i++)
		{
			serial[i + 9] = this->data[i];
		}

		return serial;
	}

	void Frame::setSOH(char SOH) {
		this->SOH = SOH;
	}

	void Frame::setSeqNum(int seqNum) {
		this->seqNum = seqNum;
	}

	void Frame::setDataLength(int dataLength) {
		this->dataLength = dataLength;
	}

	void Frame::setData(char *data) {
		for (int i = 0; i < dataLength; i++) {
			this->data[i] = data[i];
		}
	}

	char Frame::getSOH() {
		return this->SOH;
	}

	int Frame::getSeqNum() {
		return this->seqNum;
	}

	int Frame::getDataLength() {
		return this->dataLength;
	}

	char * Frame::getData() {
		return this->data;
	}

	void Frame::generateChecksum() {
		unsigned short shortSum = 0;
		unsigned char sum = 0;
		unsigned char temp = 0;
		unsigned char sumTemp = 0;

		char *serial = this->serialize();
		for (int i = 0; i < this->dataLength + 9; i += 1)
		{
			temp = (unsigned char)serial[i];
			shortSum = sum + temp;
			sum = sum + temp;
			if (shortSum >= sum)
			{
				sum += 1;
			}
		}

		this->checksum = ~sum;
	}
}