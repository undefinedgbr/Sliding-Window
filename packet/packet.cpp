#include "packet.hpp"

namespace packet {
	unsigned char Packet::getChecksum() {
		return this->checksum;
	}

	bool Packet::checkValidity() {
		char tempChecksum;
		tempChecksum = this->checksum;
		this->generateChecksum();
		return (unsigned char) this->checksum == (unsigned char) tempChecksum;
	}
}