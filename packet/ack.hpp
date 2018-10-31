#include "packet.hpp"

#ifndef ACK_H
#define ACK_H

namespace packet {
	class ACK : public Packet {
	private:
		char ack;
		int nextSeqNum;
		unsigned char checksum;
	public:
		ACK();
		ACK(char ack, int nextSeqNum);
		ACK(const ACK& ack);
		ACK(char * serial);
		char getAck();
		int getNextSeqNum();
		void setAck(char ack);
		void setNextSeqNum(int nextSeqNum);
		char * serialize() const override;
		void generateChecksum() override;
	};
}
#endif