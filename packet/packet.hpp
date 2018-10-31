#ifndef PACKET_H
#define PACKET_H

namespace packet {
	class Packet {
	protected:
		unsigned char checksum;
		int seqNum;
	public:
		virtual char * serialize() const = 0;
		virtual void generateChecksum() = 0;
		virtual unsigned char getChecksum();
		virtual bool checkValidity();
	};
}

#endif