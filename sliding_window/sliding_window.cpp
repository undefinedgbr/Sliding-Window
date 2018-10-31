#include "sliding_window.hpp"
#include <stdio.h>

namespace sw {
	using namespace packet;

	SlidingWindow::SlidingWindow(int size) {
		this->size = size;
		this->start = 0;
		this->end = this->start + size;
		this->availableFrame = size;
		this->acks.resize(size);
		this->frames.resize(size);
	}

	SlidingWindow::SlidingWindow(const SlidingWindow& window) {
		this->size = window.getSize();
		this->start = window.getStart();
		this->end = window.getEnd();
		this->availableFrame = this->size;
		this->acks.resize(this->size);
		this->frames.resize(this->size);
	}

	bool SlidingWindow::addFrame(Frame frame) {
		if (this->availableFrame == 0) {
			return false;
		}

		if (frame.getSeqNum() >= this->frames.size()) {
			return false;
		}

		if (frame.getSeqNum() > this->end) {
			return false;
		}

		this->frames[frame.getSeqNum()] = frame;
		this->availableFrame--;
	}

	bool SlidingWindow::addACK(ACK ack) {
		if (ack.getAck() != 0x6) {
			return false;
		}

		if (ack.getNextSeqNum() > this->acks.size()) {
			return false;
		}

		if (ack.getNextSeqNum() -1 > this->end) {
			return false;
		}

		this->availableFrame++;
		if (this->acks[ack.getNextSeqNum() - 1].getAck() != 0x6) {
			this->start++;
			this->end++;
			this->acks.resize(this->acks.size() + 1);
			printf("TEST ACK4\n");
			this->frames.resize(this->frames.size() + 1);
			printf("TEST ACK3\n");
		}
		printf("TEST ACK\n");
		this->acks[ack.getNextSeqNum() - 1] = ack;
		printf("TEST ACK2\n");
	}

	Frame& SlidingWindow::getFrame(int seqNum) {
		if (this->frames.size() <= seqNum) {
			return this->frames[0];
		}

		return this->frames[seqNum];
	}

	ACK& SlidingWindow::getACK(int seqNum) {
		if (this->acks.size() <= seqNum) {
			return this->acks[0];
		}

		return this->acks[seqNum];
	}

	void SlidingWindow::setSize(int size) {
		this->size = size;
	} 

	void SlidingWindow::setStart(int start) {
		this->start = start;
	}

	void SlidingWindow::setEnd(int end) {
		this->end = end;
	}

	void SlidingWindow::incrementAvailableFrame() {
		this->availableFrame++;
	}

	int SlidingWindow::getSize() const {
		return this->size;
	}

	int SlidingWindow::getStart() const {
		return this->start;
	}

	int SlidingWindow::getEnd() const {
		return this->end;
	}

	void SlidingWindow::decrementAvailableFrame() {
		this->availableFrame--;
	}
}