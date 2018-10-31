#include "packet.hpp"

#ifndef FRAME_H
#define FRAME_H


namespace packet {
	class Frame : public Packet {
	  private:
	  	static int frameCount;
		char SOH;
		int seqNum;
		char data[1024];
		int dataLength;
	  public:
	  	~Frame();
		Frame();
		Frame(char *serial);
		Frame(const Frame &frame);
		Frame(char SOH, int seqNum, int dataLength, char * data);
		char * serialize() const;
		void setSOH(char SOH);
		void setSeqNum(int seqNum);
		void setDataLength(int dataLength);
		void setData(char *data);
		char getSOH();
		int getSeqNum();
		int getDataLength();
		char *getData();
		void generateChecksum() override;
	};
}
#endif